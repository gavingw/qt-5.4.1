
#include "CameraDebug.h"
#if DBG_V4L2_CAMERA
#define LOG_NDEBUG 0
#endif
#define LOG_TAG "V4L2CameraDevice"
#include <sdklog.h>
#include <cutils/log.h>

#include <sys/mman.h> 
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/fb.h>
//#include <videodev2.h>
//#include <linux/videodev2.h> 
#include <sys/time.h>
#include "videodev2_new.h"
#include <gps.h>

#ifdef USE_MP_CONVERT
#include <g2d_driver.h>
#endif



#ifdef USE_Deinterlace_HW

#include <deinterlace_v4l2.h>
#endif

#include "V4L2CameraDevice2.h"
#include "CallbackNotifier.h"
#include "PreviewWindow.h"
#include "CameraHardware2.h"
#include "HALCameraFactory.h"

//#define CDX20 1
#define CDX_FOR_PURE_H264 1
#define MAX_ADAS_SIZE (1920*1080*3/2)

#define CHECK_NO_ERROR(a)						\
	if (a != NO_ERROR) {						\
		if (mCameraFd != NULL) {				\
			close(mCameraFd);					\
			mCameraFd = NULL;					\
		}										\
		return EINVAL;							\
	}
	
extern void PreviewCnr(unsigned int snr_level, unsigned int gain, int width, int height, char *src, char *dst);
#ifdef ASDK

#else
#define VIDEO1_MIRROR 1
#define WATERMARK_USE_MS 1
#endif
namespace android {

#ifdef SUPPORT_VIRTUAL_CAMERA
int V4L2CameraDevice::mCsiFdsC2V[2] = {-1, -1};
bool V4L2CameraDevice::mCsiRunning = false;
bool V4L2CameraDevice::mCsiVirRunning = false;
#endif
// defined in HALCameraFactory.cpp
extern void getCallingProcessName(char *name);

static void calculateCrop(Rect * rect, int new_zoom, int max_zoom, int width, int height)
{
	if (max_zoom == 0)
	{
		rect->left		= 0;
		rect->top		= 0;
		rect->right 	= width -1;
		rect->bottom	= height -1;
	}
	else
	{
		int new_ratio = (new_zoom * 2 * 100 / max_zoom + 100);
		rect->left		= (width - (width * 100) / new_ratio)/2;
		rect->top		= (height - (height * 100) / new_ratio)/2;
		rect->right 	= rect->left + (width * 100) / new_ratio -1;
		rect->bottom	= rect->top  + (height * 100) / new_ratio - 1;
	}
	
	// LOGD("crop: [%d, %d, %d, %d]", rect->left, rect->top, rect->right, rect->bottom);
}


static void YUYVToNV12(const void* yuyv, void *nv12, int width, int height)
{
	uint8_t* Y	= (uint8_t*)nv12;
	uint8_t* UV = (uint8_t*)Y + width * height;
	
	for(int i = 0; i < height; i += 2)
	{
		for (int j = 0; j < width; j++)
		{
			*(uint8_t*)((uint8_t*)Y + i * width + j) = *(uint8_t*)((uint8_t*)yuyv + i * width * 2 + j * 2);
			*(uint8_t*)((uint8_t*)Y + (i + 1) * width + j) = *(uint8_t*)((uint8_t*)yuyv + (i + 1) * width * 2 + j * 2);
			*(uint8_t*)((uint8_t*)UV + ((i * width) >> 1) + j) = *(uint8_t*)((uint8_t*)yuyv + i * width * 2 + j * 2 + 1);
		}
	}
}

static void YUYVToNV21(const void* yuyv, void *nv21, int width, int height)
{
	uint8_t* Y	= (uint8_t*)nv21;
	uint8_t* VU = (uint8_t*)Y + width * height;
	
	for(int i = 0; i < height; i += 2)
	{
		for (int j = 0; j < width; j++)
		{
			*(uint8_t*)((uint8_t*)Y + i * width + j) = *(uint8_t*)((uint8_t*)yuyv + i * width * 2 + j * 2);
			*(uint8_t*)((uint8_t*)Y + (i + 1) * width + j) = *(uint8_t*)((uint8_t*)yuyv + (i + 1) * width * 2 + j * 2);

			if (j % 2)
			{
				if (j < width - 1)
				{
					*(uint8_t*)((uint8_t*)VU + ((i * width) >> 1) + j) = *(uint8_t*)((uint8_t*)yuyv + i * width * 2 + (j + 1) * 2 + 1);
				}
			}
			else
			{
				if (j > 1)
				{
					*(uint8_t*)((uint8_t*)VU + ((i * width) >> 1) + j) = *(uint8_t*)((uint8_t*)yuyv + i * width * 2 + (j - 1) * 2 + 1); 		
				}
			}
		}
	}
}

#ifdef USE_MP_CONVERT



void V4L2CameraDevice::YUYVToYUV420C(const void* yuyv, void *yuv420, int width, int height)
{
	g2d_blt		blit_para;
	int 		err;
	
	blit_para.src_image.addr[0]      = (int)yuyv;
	blit_para.src_image.addr[1]      = (int)yuyv + width * height;
	blit_para.src_image.w            = width;	      /* src buffer width in pixel units */
	blit_para.src_image.h            = height;	      /* src buffer height in pixel units */
	blit_para.src_image.format       = G2D_FMT_IYUV422;
	blit_para.src_image.pixel_seq    = G2D_SEQ_NORMAL;          /* not use now */
	blit_para.src_rect.x             = 0;						/* src rect->x in pixel */
	blit_para.src_rect.y             = 0;						/* src rect->y in pixel */
	blit_para.src_rect.w             = width;			/* src rect->w in pixel */
	blit_para.src_rect.h             = height;			/* src rect->h in pixel */

	blit_para.dst_image.addr[0]      = (int)yuv420;
	blit_para.dst_image.addr[1]      = (int)yuv420 + width * height;
	blit_para.dst_image.w            = width;	      /* dst buffer width in pixel units */			
	blit_para.dst_image.h            = height;	      /* dst buffer height in pixel units */
	blit_para.dst_image.format       = G2D_FMT_PYUV420UVC;
	blit_para.dst_image.pixel_seq    = (mVideoFormat == V4L2_PIX_FMT_NV12) ? G2D_SEQ_NORMAL : G2D_SEQ_VUVU;          /* not use now */
	blit_para.dst_x                  = 0;					/* dst rect->x in pixel */
	blit_para.dst_y                  = 0;					/* dst rect->y in pixel */
	blit_para.color                  = 0xff;          		/* fix me*/
	blit_para.alpha                  = 0xff;                /* globe alpha */ 

	blit_para.flag = G2D_BLT_NONE; // G2D_BLT_FLIP_HORIZONTAL;

	err = ioctl(mG2DHandle, G2D_CMD_BITBLT, (unsigned long)&blit_para);				
	if(err < 0) 	
	{			
		LOGE("ioctl, G2D_CMD_BITBLT failed");
		return;
	}
}

void V4L2CameraDevice::NV21ToYV12(const void* nv21, void *yv12, int width, int height)
{
	g2d_blt		blit_para;
	int 		err;
	int			u, v;
	if (mVideoFormat == V4L2_PIX_FMT_NV21)
	{
		u = 1;
		v = 2;
	}
	else
	{
		u = 2;
		v = 1;
	}
	
	blit_para.src_image.addr[0]      = (int)nv21;
	blit_para.src_image.addr[1]      = (int)nv21 + width * height;
	blit_para.src_image.w            = width;	      /* src buffer width in pixel units */
	blit_para.src_image.h            = height;	      /* src buffer height in pixel units */
	blit_para.src_image.format       = G2D_FMT_PYUV420UVC;
	blit_para.src_image.pixel_seq    = G2D_SEQ_NORMAL;//G2D_SEQ_VUVU;          /*  */
	blit_para.src_rect.x             = 0;						/* src rect->x in pixel */
	blit_para.src_rect.y             = 0;						/* src rect->y in pixel */
	blit_para.src_rect.w             = width;			/* src rect->w in pixel */
	blit_para.src_rect.h             = height;			/* src rect->h in pixel */

	blit_para.dst_image.addr[0]      = (int)yv12;							// y
	blit_para.dst_image.addr[u]      = (int)yv12 + width * height;			// v
	blit_para.dst_image.addr[v]      = (int)yv12 + width * height * 5 / 4;	// u
	blit_para.dst_image.w            = width;	      /* dst buffer width in pixel units */			
	blit_para.dst_image.h            = height;	      /* dst buffer height in pixel units */
	blit_para.dst_image.format       = G2D_FMT_PYUV420;
	blit_para.dst_image.pixel_seq    = G2D_SEQ_NORMAL;          /* not use now */
	blit_para.dst_x                  = 0;					/* dst rect->x in pixel */
	blit_para.dst_y                  = 0;					/* dst rect->y in pixel */
	blit_para.color                  = 0xff;          		/* fix me*/
	blit_para.alpha                  = 0xff;                /* globe alpha */ 
	
	blit_para.flag = G2D_BLT_NONE;
	
	err = ioctl(mG2DHandle , G2D_CMD_BITBLT, (unsigned long)&blit_para);				
	if(err < 0)
	{
		LOGE("NV21ToYV12 ioctl, G2D_CMD_BITBLT failed");
		return;
	}
}

void V4L2CameraDevice::NV21Rotate(const void* nv21, void *dst, int width, int height)
{
	g2d_blt		blit_para;
	int 		err;

	blit_para.src_image.addr[0]      = (int)nv21;
	blit_para.src_image.addr[1]      = (int)nv21 + width * height;
	blit_para.src_image.w            = width;	      /* src buffer width in pixel units */
	blit_para.src_image.h            = height;	      /* src buffer height in pixel units */
	blit_para.src_image.format       = G2D_FMT_PYUV420UVC;
	blit_para.src_image.pixel_seq    = G2D_SEQ_NORMAL;//G2D_SEQ_VUVU;          /*  */
	blit_para.src_rect.x             = 0;						/* src rect->x in pixel */
	blit_para.src_rect.y             = 0;						/* src rect->y in pixel */
	blit_para.src_rect.w             = width;			/* src rect->w in pixel */
	blit_para.src_rect.h             = height;			/* src rect->h in pixel */

	blit_para.dst_image.addr[0]      = (int)dst;							// y
#if 0	
	blit_para.dst_image.addr[1]      = (int)dst + width * height;			// vu
	blit_para.dst_image.format       = G2D_FMT_PYUV420UVC;
#else
	blit_para.dst_image.addr[1]      = (int)dst + width * height;			// v
	blit_para.dst_image.addr[2]      = (int)dst + width * height * 5 / 4;	// u
	blit_para.dst_image.format       = G2D_FMT_PYUV420;
#endif	
	blit_para.dst_image.w            = height;	      /* dst buffer width in pixel units */			
	blit_para.dst_image.h            = width;	      /* dst buffer height in pixel units */

	blit_para.dst_image.pixel_seq    = G2D_SEQ_NORMAL;          /* not use now */
	blit_para.dst_x                  = 0;					/* dst rect->x in pixel */
	blit_para.dst_y                  = 0;					/* dst rect->y in pixel */
	blit_para.color                  = 0xff;          		/* fix me*/
	blit_para.alpha                  = 0xff;                /* globe alpha */ 
	
	blit_para.flag =G2D_BLT_ROTATE90;//G2D_BLT_NONE; //
	
	err = ioctl(mG2DHandle , G2D_CMD_BITBLT, (unsigned long)&blit_para);				
	if(err < 0)
	{
		LOGE("NV21Rotate ioctl, G2D_CMD_BITBLT failed");
		return;
	}
}

#endif

#ifdef USE_Deinterlace_HW

void V4L2CameraDevice::Hw_Di_Process(unsigned char *presrc, int pre_width, int pre_height,
unsigned char *src, int src_width, int src_height, unsigned char *dst, int dst_width, int dst_height,int nField)
{

	int ret = 0;
    DiParaT        mDiParaT;
	DiRectSizeT    mSrcSize;
	DiRectSizeT    mDstSize;
	DiPixelformatE eInFormat;
	DiPixelformatE eOutFormat;
	eOutFormat = DI_FORMAT_NV21;
	eInFormat = DI_FORMAT_NV21;
	mSrcSize.nWidth  = src_width;
	mSrcSize.nHeight = src_height;
	mDstSize.nWidth  = dst_width;
	mDstSize.nHeight = dst_height;
	mDiParaT.mInputFb.mRectSize.nWidth= src_width;
	mDiParaT.mInputFb.mRectSize.nHeight= src_height;
	mDiParaT.mInputFb.eFormat	 = DI_FORMAT_NV21;	
	mDiParaT.mPreFb.mRectSize.nWidth = pre_width;
	mDiParaT.mPreFb.mRectSize.nHeight= pre_height;
	mDiParaT.mPreFb.eFormat 	 = DI_FORMAT_NV21;	
	mDiParaT.mOutputFb.mRectSize.nWidth = dst_width;
	mDiParaT.mOutputFb.mRectSize.nHeight= dst_height;
	mDiParaT.mOutputFb.eFormat	 = DI_FORMAT_NV21;
	mDiParaT.mSourceRegion.nWidth= src_width;
	mDiParaT.mSourceRegion.nHeight= src_height;
	mDiParaT.mOutRegion.nWidth = dst_width;
	mDiParaT.mOutRegion.nHeight = dst_height;	
	mDiParaT.nField 			 = nField;
	mDiParaT.bTopFieldFirst 	 = 1;
   	mDiParaT.mInputFb.addr[0]    = (int)src;
	mDiParaT.mInputFb.addr[1]    = (int)src + src_width*src_height;
	mDiParaT.mPreFb.addr[0]      = (int)presrc;
	mDiParaT.mPreFb.addr[1]      = (int)presrc+pre_width*pre_height;
	mDiParaT.mOutputFb.addr[0]   = (int)dst;
	mDiParaT.mOutputFb.addr[1]   = (int)dst+dst_width*dst_height;

	ret = ioctl(mdiHandle , DI_IOCSTART, (unsigned long)&mDiParaT);		
	
	if(ret < 0)
	{
		LOGE("hw di ioctl, DI_IOCSTART failed");
		return;
	}

}



#endif
DBG_TIME_AVG_BEGIN(TAG_CONTINUOUS_PICTURE);
static int saveframe(char *str,void *p,int length,int is_oneframe)
{
	FILE *fd;

	if(is_oneframe){
		fd = fopen(str,"wb");
	}
	else{
		fd = fopen(str,"a");
	}

	if(!fd){
		LOGE("Open file error");
		return -1;
	}
	if(fwrite(p,1,length,fd)){
	   LOGD("Write file successfully");
	   fclose(fd);
	   return 0;
	}
	else {
	   LOGE("Write file fail");
	   fclose(fd);
	   return -1;
	}
}

void  V4L2CameraDevice::showformat(int format,char *str)
{
	switch(format){
	case V4L2_PIX_FMT_YUYV:
		LOGD("The %s foramt is V4L2_PIX_FMT_YUYV",str);
		break;
	case V4L2_PIX_FMT_MJPEG:
		LOGD("The %s foramt is V4L2_PIX_FMT_MJPEG",str);
		break;
	case V4L2_PIX_FMT_YVU420:
		LOGD("The %s foramt is V4L2_PIX_FMT_YVU420",str);
		break;
	case V4L2_PIX_FMT_NV12:
		LOGD("The %s foramt is V4L2_PIX_FMT_NV12",str);
		break;
	case V4L2_PIX_FMT_NV21:
		LOGD("The %s foramt is V4L2_PIX_FMT_NV21",str);
		break;
	case V4L2_PIX_FMT_H264:
		LOGD("The %s foramt is V4L2_PIX_FMT_H264",str);
		break;
	default:
		LOGD("The %s format can't be showed",str);
	}
}
V4L2CameraDevice::V4L2CameraDevice(CameraHardware* camera_hal,
								   PreviewWindow * preview_window, 
    							   CallbackNotifier * cb)
    : mCameraHardware(camera_hal),
      mPreviewWindow(preview_window),
      mCallbackNotifier(cb),
      mCameraDeviceState(STATE_CONSTRUCTED),
      mCaptureThreadState(CAPTURE_STATE_NULL),
      mCameraFd(0),
      mCameraType(CAMERA_TYPE_CSI),
      mTakePictureState(TAKE_PICTURE_NULL),
      mIsPicCopy(false),
      mFrameWidth(0),
      mFrameHeight(0),
      mThumbWidth(0),
      mThumbHeight(0),
      mCurFrameTimestamp(0),
      mBufferCnt(NB_BUFFER),
      mUseHwEncoder(false),
	  mNewZoom(0),
	  mLastZoom(-1),
	  mMaxZoom(0xffffffff),
	  mCaptureFormat(V4L2_PIX_FMT_NV21),
	  mVideoFormat(V4L2_PIX_FMT_NV21)
#ifdef USE_MP_CONVERT
	  ,mG2DHandle(0)
#endif
#ifdef USE_Deinterlace_HW
     ,mdiHandle(0)
    
#endif
	  ,mCurrentV4l2buf(NULL)
	  ,mCanBeDisconnected(false)
	  ,mContinuousPictureStarted(false)
	  ,mContinuousPictureCnt(0)
	  ,mContinuousPictureMax(0)
	  ,mContinuousPictureStartTime(0)
	  ,mContinuousPictureLast(0)
	  ,mContinuousPictureAfter(0)
	  ,mSmartPictureDone(true)
	  ,mFaceDectectLast(0)
	  ,mFaceDectectAfter(0)
	  ,mPreviewLast(0)
	  ,mPreviewAfter(0)
	  ,mVideoHint(false)
	  ,mCurAvailBufferCnt(0)
	  ,mStatisicsIndex(0)
	  ,mNeedHalfFrameRate(false)
	  ,mShouldPreview(true)
	  ,mIsThumbUsedForVideo(false)
	  ,mVideoWidth(640)
	  ,mVideoHeight(480)
	  ,mFrameRate(30)
      ,mDecoder(NULL)
      ,mCameraList(NULL)
      ,mPicCnt(0)
      ,mOrgTime(0)
#ifdef ADAS_ENABLE
	  ,mCurV4l2Buf(NULL)	
	  ,mAdasStarted(false)
#endif
	  ,adasIgnorFrameCnt(0)

#ifdef WATERMARK
	  ,mWaterMarkEnable(false)
	  ,mWaterMarkCtrlRec(NULL)
	  ,mWaterMarkCtrlPrev(NULL)
	  ,mWaterMarkMultiple(NULL)
      ,mWaterMarkDispMode(WATER_MARK_DISP_MODE_VIDEO_AND_PREVIEW)
#endif
	  ,mCameraFlipStatus(NO_FLIP)
#if 1
//def CAMERA_MANAGER_ENABLE
	  ,mCameraManager(NULL)
#endif
{
	LOGV("V4L2CameraDevice construct");

	memset(&mHalCameraInfo, 0, sizeof(mHalCameraInfo));
	memset(&mRectCrop, 0, sizeof(Rect));
	memset(&mMapMem, 0, sizeof(v4l2_mem_map_t));
	memset(&mVideoBuffer, 0, sizeof(bufferManagerQ_t));
	memset(&mVideoConf, 0, sizeof(VConfig));
	memset(&mVideoInfo, 0, sizeof(VideoStreamInfo));
	memset(&mDataInfo, 0, sizeof(VideoStreamDataInfo));

	// init preview buffer queue
	OSAL_QueueCreate(&mQueueBufferPreview, NB_BUFFER);
	OSAL_QueueCreate(&mQueueBufferPicture, 4);
	OSAL_QueueCreate(&mQueueBufferDi, 4);
    mGpsData = NULL;
#ifdef WATERMARK
	pthread_mutex_init(&mWaterMarkLock, NULL);
#endif
#ifdef ADAS_ENABLE
	pthread_mutex_init(&mAdasMutex, NULL);
	pthread_cond_init(&mAdasCond,NULL);
#endif
	pthread_mutex_init(&mConnectMutex, NULL);
	pthread_cond_init(&mConnectCond, NULL);
	// init capture thread
	mCaptureThread = new DoCaptureThread(this);
	pthread_mutex_init(&mCaptureMutex, NULL);
	pthread_cond_init(&mCaptureCond, NULL);
	pthread_mutex_lock(&mCaptureMutex);
	mCaptureThreadState = CAPTURE_STATE_PAUSED;
	pthread_mutex_unlock(&mCaptureMutex);
	mCaptureThread->startThread();

	// init preview thread
	mPreviewThread = new DoPreviewThread(this);
	pthread_mutex_init(&mPreviewMutex, NULL);
	pthread_cond_init(&mPreviewCond, NULL);
	mPreviewThread->startThread();

	// init picture thread
	mPictureThread = new DoPictureThread(this);
	pthread_mutex_init(&mPictureMutex, NULL);
	pthread_cond_init(&mPictureCond, NULL);
	mPictureThread->startThread();
	
	pthread_mutex_init(&mConnectMutex, NULL);
	pthread_cond_init(&mConnectCond, NULL);
	
	// init continuous picture thread
	mContinuousPictureThread = new DoContinuousPictureThread(this);
	pthread_mutex_init(&mContinuousPictureMutex, NULL);
	pthread_cond_init(&mContinuousPictureCond, NULL);
	mContinuousPictureThread->startThread();

	// init smart picture thread
	mSmartPictureThread = new DoSmartPictureThread(this);
	pthread_mutex_init(&mSmartPictureMutex, NULL);
	pthread_cond_init(&mSmartPictureCond, NULL);
	mSmartPictureThread->startThread();

}
#ifdef ADAS_ENABLE
status_t V4L2CameraDevice::adasInitialize()
{
	if(mAdasStarted)
	{
		ALOGW("adas has inited!");
		return 0;
	}
	adasIgnorFrameCnt =0;
	mAdasThread = new DoAdasThread(this);
	mAdasThread->startThread();
	mAdasStarted = true;
	return 0;
}

unsigned char image_buf[1920*1080*3/2];

bool V4L2CameraDevice::adasThread()
{
#if 0
	unsigned char *image_buf;
	pthread_mutex_lock(&mAdasMutex);
	pthread_cond_wait(&mAdasCond, &mAdasMutex);
	image_buf = mCurV4l2Buf;
	if (mAdasStarted && (image_buf != NULL)) fcw_execute(image_buf);
	pthread_mutex_unlock(&mAdasMutex);
#else
	pthread_mutex_lock(&mAdasMutex);
	pthread_cond_wait(&mAdasCond, &mAdasMutex);
	if ( mCurV4l2Buf != NULL ) memcpy(image_buf,mCurV4l2Buf,ADAS_WIDTH*ADAS_HEIGHT*3/2);
	if (mAdasStarted) fcw_execute(image_buf);
	pthread_mutex_unlock(&mAdasMutex);
#endif
	ADASOUT_IF adasResult;
	memcpy(&adasResult.ldw_result,&g_ldw_result,sizeof(g_ldw_result));
	memcpy(&adasResult.fcw_result,&g_fcw_result,sizeof(g_fcw_result));
	//ALOGE("adasThread##########vanishLine =%f",adasResult.ldw_result.autoCalVanishLine);
	mCallbackNotifier->adasResultMsg(&adasResult);
	return true;
}


void V4L2CameraDevice::adasDestroy()
{

	if (!mAdasStarted) {
		ALOGW("Adas is not running yet!");
		return;
	}
	ALOGI("<adasDestroy>start");
	mAdasStarted = false;
	pthread_mutex_lock(&mAdasMutex);
	fcw_close();
	pthread_mutex_unlock(&mAdasMutex);
	if ( mAdasThread != NULL ) {
		mAdasThread->stopThread();
		pthread_mutex_lock(&mAdasMutex);
		pthread_cond_signal(&mAdasCond);
		pthread_mutex_unlock(&mAdasMutex);
		mAdasThread.clear();
		mAdasThread = 0;
			
	}
	ALOGI("<adasDestroy>end");
}


int V4L2CameraDevice::setAdasParameters(int key,int value)
{
	return fcw_setAdasParameters(key,value);
}
#endif
V4L2CameraDevice::~V4L2CameraDevice()
{
	LOGV("V4L2CameraDevice disconstruct");

	if (mCaptureThread != NULL)
	{
		mCaptureThread->stopThread();
		pthread_cond_signal(&mCaptureCond);
		mCaptureThread.clear();
		mCaptureThread = 0;
	}

	if (mPreviewThread != NULL)
	{
		mPreviewThread->stopThread();
		pthread_cond_signal(&mPreviewCond);
		mPreviewThread.clear();
		mPreviewThread = 0;
	}

	if (mPictureThread != NULL)
	{
		mPictureThread->stopThread();
		pthread_cond_signal(&mPictureCond);
		mPictureThread.clear();
		mPictureThread = 0;
	}

	if (mContinuousPictureThread != NULL)
	{
		mContinuousPictureThread->stopThread();
		pthread_cond_signal(&mContinuousPictureCond);
		mContinuousPictureThread.clear();
		mContinuousPictureThread = 0;
	}

	if (mSmartPictureThread != NULL)
	{
		mSmartPictureThread->stopThread();
		pthread_cond_signal(&mSmartPictureCond);
		mSmartPictureThread.clear();
		mSmartPictureThread = 0;
	}

	pthread_mutex_destroy(&mCaptureMutex);
	pthread_cond_destroy(&mCaptureCond);

	pthread_mutex_destroy(&mPreviewMutex);
	pthread_cond_destroy(&mPreviewCond);
	
	pthread_mutex_destroy(&mPictureMutex);
	pthread_cond_destroy(&mPictureCond);
	
	pthread_mutex_destroy(&mConnectMutex);
	pthread_cond_destroy(&mConnectCond);
	
	pthread_mutex_destroy(&mContinuousPictureMutex);
	pthread_cond_destroy(&mContinuousPictureCond);

	pthread_mutex_destroy(&mSmartPictureMutex);
	pthread_cond_destroy(&mSmartPictureCond);
	
	OSAL_QueueTerminate(&mQueueBufferPreview);
	OSAL_QueueTerminate(&mQueueBufferPicture);
	OSAL_QueueTerminate(&mQueueBufferDi);
#ifdef WATERMARK
	pthread_mutex_destroy(&mWaterMarkLock);
#endif
#ifdef ADAS_ENABLE
	pthread_mutex_destroy(&mAdasMutex);
	pthread_cond_destroy(&mAdasCond);
#endif

}

/****************************************************************************
 * V4L2CameraDevice interface implementation.
 ***************************************************************************/

void V4L2CameraDevice::setCameraFlipStatus(int flipEnable)
{
    mCameraFlipStatus = flipEnable;
    LOGV("%s : mCameraFlipStatus = %d", __FUNCTION__, mCameraFlipStatus);
}

status_t V4L2CameraDevice::connectDevice(HALCameraInfo * halInfo)
{
	F_LOG;

    Mutex::Autolock locker(&mObjectLock);
	
	if (isConnected()) 
	{
		LOGW("%s: camera device is already connected.", __FUNCTION__);
		return NO_ERROR;
	}
	LOGV("begin openCameraDev");

	// open v4l2 camera device
	int ret = openCameraDev(halInfo);
	if (ret != OK)
	{
		return ret;
	}
	LOGV("begin openCameraDev done");

	memcpy((void*)&mHalCameraInfo, (void*)halInfo, sizeof(HALCameraInfo));

#ifdef USE_MP_CONVERT
	//if (mCameraType == CAMERA_TYPE_UVC)
	{
		// open MP driver
		mG2DHandle = open("/dev/g2d", O_RDWR, 0);
		if (mG2DHandle < 0)
		{
			LOGE("open /dev/g2d failed");
			goto END_ERROR;
		}
		LOGV("open /dev/g2d OK");
	}
#endif 

#ifdef USE_Deinterlace_HW
	if (mCameraType == CAMERA_TYPE_TVIN_NTSC||mCameraType == CAMERA_TYPE_TVIN_PAL||mCameraType == CAMERA_TYPE_TVIN_YPBPR)
	{
#ifdef CAMERA_MANAGER_ENABLE
		if((mCameraManager != NULL) && (mCameraManager->isOviewEnable()))
		{
		}
		else
#endif
		{
			// open deinterlace driver
			mdiHandle = open("/dev/deinterlace", O_RDWR, 0);
			if (mdiHandle < 0)
			{
				LOGE("open /dev/deinterlacefailed");
				goto END_ERROR;
			}
			LOGV("open /dev/deinterlace OK");
		}
		
	}	
	
#endif

#ifdef USE_ION_MEM_ALLOCATOR
	ret = ion_alloc_open();
	if (ret < 0)
	{
		LOGE("ion_alloc_open failed");
		goto END_ERROR;
	}
	LOGV("ion_alloc_open ok");
#ifdef SUPPORT_VIRTUAL_CAMERA
    if (mCameraType == CAMERA_TYPE_VIR) {
        mPicBuffer.addrVirY = 0;
        mPicBuffer.addrPhyY = 0;
        mCameraDeviceState = STATE_CONNECTED;
        return NO_ERROR;
    }
#endif
	// used for normal picture mode
	mPicBuffer.addrVirY = (int)ion_alloc_alloc(MAX_PICTURE_SIZE);
	mPicBuffer.addrPhyY = ion_alloc_vir2phy((void*)mPicBuffer.addrVirY);
#ifdef USE_MP_CONVERT
	mG2dAddrVir = (int)ion_alloc_alloc(BUFFER_SIZE);
	mG2dAddrPhy = ion_alloc_vir2phy((void*)mG2dAddrVir);
#endif
#ifdef ADAS_ENABLE
	mAdasBuffer.addrVirY = (int)ion_alloc_alloc(MAX_ADAS_SIZE);
	mAdasBuffer.addrPhyY = ion_alloc_vir2phy((void*)mAdasBuffer.addrVirY);
#endif
 #ifdef USE_Deinterlace_HW
   mdiBuffer.addrVirY = (int)ion_alloc_alloc(MAX_DI_SIZE);
   mdiBuffer.addrPhyY = ion_alloc_vir2phy((void*)mdiBuffer.addrVirY);
   mdiBuffer_pre.addrVirY = (int)ion_alloc_alloc(MAX_DI_SIZE);
   mdiBuffer_pre.addrPhyY = ion_alloc_vir2phy((void*)mdiBuffer_pre.addrVirY);
   
   #endif
#elif  USE_CDX_NEW_MEM_API
	//mCameraMemops=
	LOGV("begin MemAdapterGetOpsS");
	mCameraMemops = MemAdapterGetOpsS();	 
	if(mCameraMemops == NULL)	  { LOGE("camera MemAdapterGetOpsS failed\n");		  }    
	
	ret = CdcMemOpen(mCameraMemops);

	if (ret < 0)
	{
		LOGE("ion_alloc_open failed");
		goto END_ERROR;
	}
	mCallbackNotifier->setScMemOpsS(mCameraMemops);
	LOGV("MemAdapterGetOpsS ok");
#ifdef SUPPORT_VIRTUAL_CAMERA
    if (mCameraType == CAMERA_TYPE_VIR) {
        mPicBuffer.addrVirY = 0;
        mPicBuffer.addrPhyY = 0;
        mCameraDeviceState = STATE_CONNECTED;
        return NO_ERROR;
    }
#endif
#if 1
//def CAMERA_MANAGER_ENABLE
			if((mCameraManager != NULL) && (mCameraManager->isOviewEnable()))
			{
			//mCameraManager->setScMemOpsS(mCameraMemops);
			}
			else
#endif
	{
	// used for normal picture mode
	mPicBuffer.addrVirY = (int)CdcMemPalloc(mCameraMemops, MAX_PICTURE_SIZE);
	mPicBuffer.addrPhyY = (unsigned int)CdcMemGetPhysicAddressCpu(mCameraMemops,(void*)mPicBuffer.addrVirY);
#ifdef USE_MP_CONVERT
	//mG2dAddrVir = (int)CdcMemPalloc(mCameraMemops, BUFFER_SIZE);
	//mG2dAddrPhy = (unsigned int)CdcMemGetPhysicAddressCpu(mCameraMemops,(void*)mG2dAddrVir);
#endif
#ifdef ADAS_ENABLE
	mAdasBuffer.addrVirY = (int)CdcMemPalloc(mCameraMemops, MAX_ADAS_SIZE);
	mAdasBuffer.addrPhyY = (unsigned int)CdcMemGetPhysicAddressCpu(mCameraMemops,(void*)mAdasBuffer.addrVirY);
#endif
 #ifdef USE_Deinterlace_HW
   mdiBuffer.addrVirY = (int)CdcMemPalloc(mCameraMemops, MAX_DI_SIZE);
   mdiBuffer.addrPhyY = (int)CdcMemGetPhysicAddressCpu(mCameraMemops,(void*)mdiBuffer.addrVirY);
   mdiBuffer_pre.addrVirY = (int)CdcMemPalloc(mCameraMemops, MAX_DI_SIZE);
   mdiBuffer_pre.addrPhyY = (int)CdcMemGetPhysicAddressCpu(mCameraMemops,(void*)mdiBuffer_pre.addrVirY);
   
   #endif
	}
#else USE_SUNXI_MEM_ALLOCATOR
	ret = sunxi_alloc_open();
	if (ret < 0)
	{
		LOGE("sunxi_alloc_open failed");
		goto END_ERROR;
	}
	LOGV("sunxi_alloc_open ok");

	// used for normal picture mode
	mPicBuffer.addrVirY = (int)sunxi_alloc_alloc(MAX_PICTURE_SIZE);
	mPicBuffer.addrPhyY = sunxi_alloc_vir2phy((void*)mPicBuffer.addrVirY);
	#ifdef ADAS_ENABLE
	mAdasBuffer.addrVirY = (int)sunxi_alloc_alloc(MAX_ADAS_SIZE);
	mAdasBuffer.addrPhyY = sunxi_alloc_vir2phy((void*)mAdasBuffer.addrVirY);
	#endif

#endif

	/* There is a device to connect to. */
	mCameraDeviceState = STATE_CONNECTED;

    return NO_ERROR;
	
END_ERROR:

	if (mCameraFd != NULL)
	{
		close(mCameraFd);
		mCameraFd = NULL;
	}
	
#ifdef USE_MP_CONVERT
	if(mG2DHandle != NULL)
	{
		close(mG2DHandle);
		mG2DHandle = NULL;
	}
#endif
#ifdef USE_Deinterlace_HW
	if(mdiHandle != NULL)
	{
		close(mdiHandle);
		mdiHandle = NULL;
	}
#endif



	return UNKNOWN_ERROR;
}

status_t V4L2CameraDevice::disconnectDevice()
{
	F_LOG;
	
	Mutex::Autolock locker(&mObjectLock);
	
	if (!isConnected()) 
	{
		LOGW("%s: camera device is already disconnected.", __FUNCTION__);
		return NO_ERROR;
	}
	
	if (isStarted()) 
	{
		LOGE("%s: Cannot disconnect from the started device.", __FUNCTION__);
		return -EINVAL;
	}
	
	// close v4l2 camera device
	closeCameraDev();
	
#ifdef USE_MP_CONVERT
	if(mG2DHandle != NULL)
	{
		close(mG2DHandle);
		mG2DHandle = NULL;
	}
#endif
#ifdef USE_Deinterlace_HW
	if(mdiHandle != NULL)
	{
		close(mdiHandle);
		mdiHandle = NULL;
	}
#endif

#ifdef USE_ION_MEM_ALLOCATOR
        if (mPicBuffer.addrVirY != NULL)
	{
		ion_alloc_free((void*)mPicBuffer.addrVirY);
		mPicBuffer.addrPhyY = 0;
	}
#ifdef USE_MP_CONVERT
	if(mG2dAddrVir != NULL)
	{
		ion_alloc_free((void*)mG2dAddrVir);
		mG2dAddrPhy = 0;
	}
#endif
#ifdef ADAS_ENABLE
	if (mAdasBuffer.addrVirY != NULL)
	{
		ion_alloc_free((void*)mAdasBuffer.addrVirY);
		mAdasBuffer.addrPhyY = 0;
	}
#endif
#ifdef USE_Deinterlace_HW
	if(mdiBuffer.addrVirY !=NULL)
		{
		ion_alloc_free((void*)mdiBuffer.addrVirY);
		mdiBuffer.addrPhyY = 0;
	}
	if(mdiBuffer_pre.addrVirY !=NULL)
		{
		ion_alloc_free((void*)mdiBuffer_pre.addrVirY);
		mdiBuffer_pre.addrPhyY = 0;
	}
#endif
	int ret = ion_alloc_close();
	if (ret < 0)
	{
		LOGE("ion_alloc_close failed\n");
	}
	else
	{
		LOGV("ion_alloc_close ok");
	}
#elif USE_CDX_NEW_MEM_API
        if (mPicBuffer.addrVirY != NULL)
	{
		CdcMemPfree(mCameraMemops,(void*)mPicBuffer.addrVirY);
		mPicBuffer.addrPhyY = 0;
	}
#ifdef USE_MP_CONVERT
	//if(mG2dAddrVir != NULL)
	//{
		//CdcMemPfree(mCameraMemops,(void*)mG2dAddrVir);
		//mG2dAddrPhy = 0;
	//}
#endif
#ifdef ADAS_ENABLE
	if (mAdasBuffer.addrVirY != NULL)
	{
		CdcMemPfree(mCameraMemops,(void*)mAdasBuffer.addrVirY);
		mAdasBuffer.addrPhyY = 0;
	}
#endif
#ifdef USE_Deinterlace_HW
	if(mdiBuffer.addrVirY !=NULL)
		{
		CdcMemPfree(mCameraMemops,(void*)mdiBuffer.addrVirY);
		mdiBuffer.addrPhyY = 0;
	}
	if(mdiBuffer_pre.addrVirY !=NULL)
		{
		CdcMemPfree(mCameraMemops,(void*)mdiBuffer_pre.addrVirY);
		mdiBuffer_pre.addrPhyY = 0;
	}
#endif
	CdcMemClose(mCameraMemops);

	mCameraMemops=NULL;

#elif USE_SUNXI_MEM_ALLOCATOR
	if (mPicBuffer.addrVirY != NULL)
	{
		sunxi_alloc_free((void*)mPicBuffer.addrVirY);
		mPicBuffer.addrPhyY = 0;
	}
#ifdef ADAS_ENABLE
	if (mAdasBuffer.addrVirY != NULL)
	{
		sunxi_alloc_free((void*)mAdasBuffer.addrVirY);
		mAdasBuffer.addrPhyY = 0;
	}
#endif

	int ret = sunxi_alloc_close();
	if (ret < 0)
	{
		LOGE("sunxi_alloc_close failed\n");
	}
	else
	{
		LOGV("sunxi_alloc_close ok");
	}
#endif

    /* There is no device to disconnect from. */
    mCameraDeviceState = STATE_CONSTRUCTED;

    return NO_ERROR;
}

status_t V4L2CameraDevice::startDevice(int width,
                                       int height,
                                       uint32_t pix_fmt,
                                       bool video_hint)
{
	LOGD("%s, wxh: %dx%d, fmt: %d", __FUNCTION__, width, height, pix_fmt);
	
	Mutex::Autolock locker(&mObjectLock);
	
	if (!isConnected()) 
	{
		LOGE("%s: camera device is not connected.", __FUNCTION__);
		return EINVAL;
	}
	
	if (isStarted()) 
	{
		LOGE("%s: camera device is already started.", __FUNCTION__);
		return EINVAL;
	}
#ifdef SUPPORT_VIRTUAL_CAMERA
    if (mCameraType == CAMERA_TYPE_VIR && mCsiRunning) {
        mCanBeDisconnected = false;
        mCameraDeviceState = STATE_STARTED;
        mCsiVirRunning = true;
        return NO_ERROR;
    }
#endif

	// VE encoder need this format
	mVideoFormat = pix_fmt;
	mCurrentV4l2buf = NULL;
#ifdef ADAS_ENABLE
	mCurV4l2Buf = NULL;
#endif

	mVideoHint = video_hint;
	mCanBeDisconnected = false;

	for(int i = 0; i < NB_BUFFER; i++)
	{
		memset(&mV4l2buf[i], 0, sizeof(V4L2BUF_t));
	}
	
	// set v4l2 device parameters, it maybe change the value of mFrameWidth and mFrameHeight.
//	CHECK_NO_ERROR(v4l2SetVideoParams(width, height, pix_fmt));
      
	v4l2SetVideoParams(width, height, pix_fmt);/*for uvc usb by */

	// set capture mode and fps
	// CHECK_NO_ERROR(v4l2setCaptureParams());	// do not check this error
	//v4l2SetVideoParams must before at v4l2setCaptureParams
	v4l2setCaptureParams();
		
	// v4l2 request buffers
	int buf_cnt = (mTakePictureState == TAKE_PICTURE_NORMAL) ? 1 : NB_BUFFER;
	CHECK_NO_ERROR(v4l2ReqBufs(&buf_cnt));
	mBufferCnt = buf_cnt;
	mCurAvailBufferCnt = mBufferCnt;

	// v4l2 query buffers
	CHECK_NO_ERROR(v4l2QueryBuf());

#if 1
//def CAMERA_MANAGER_ENABLE
	if((mCameraManager != NULL)&&(mCameraManager->isOviewEnable()))
	{
		//printf("----startDevice-x-%d --h-%d------\r\n",mFrameWidth,mFrameHeight);
		mCameraManager->setFrameSize(mHalCameraInfo.device_id,mFrameWidth,mFrameHeight);
	}
#endif
	// stream on the v4l2 device
	CHECK_NO_ERROR(v4l2StartStreaming());

	mCameraDeviceState = STATE_STARTED;

	mContinuousPictureAfter = 1000000 / 10;
	mFaceDectectAfter = 1000000 / 15;
	mPreviewAfter = 1000000 / 24;
#ifdef CDX10
    if((mCaptureFormat == V4L2_PIX_FMT_MJPEG)||(mCaptureFormat == V4L2_PIX_FMT_H264))
    {
        mStream_info.format             = (mCaptureFormat == V4L2_PIX_FMT_MJPEG) ? CEDARV_STREAM_FORMAT_MJPEG : CEDARV_STREAM_FORMAT_H264 ;
        mStream_info.sub_format         = CEDARV_SUB_FORMAT_UNKNOW;
        mStream_info.container_format   = CEDARV_CONTAINER_FORMAT_UNKNOW;
        mStream_info.video_width     = mFrameWidth;
        mStream_info.video_height    = mFrameHeight;
        mStream_info.frame_rate      = mFrameRate*1000;
        mStream_info.frame_duration  = 0;
        //stream_info.aspect_ratio   = 800/480*1000;
        mStream_info.init_data_len   = 0;
        mStream_info.init_data       = 0;
        Libve_init(&mDecoder,&mStream_info,0,&mDecoderMutex);
    }
#endif

#ifdef CDX20

	if((mCaptureFormat == V4L2_PIX_FMT_MJPEG)||((mCaptureFormat == V4L2_PIX_FMT_H264) && (SONIX_CAMERA_H264 == 0)))
	{
		LOGD("FUNC:%s, Line:%d init Dec!",__FUNCTION__,__LINE__);
		//* all decoder support YV12 format.
		mVideoConf.eOutputPixelFormat  = PIXEL_FORMAT_NV21;//PIXEL_FORMAT_YV12;PIXEL_FORMAT_YUV_MB32_420;//
		//* never decode two picture when decoding a thumbnail picture.
		mVideoConf.bDisable3D		  = 1;

		mVideoConf.nVbvBufferSize	  = 0;

		mVideoInfo.eCodecFormat =(mCaptureFormat == V4L2_PIX_FMT_MJPEG) ? VIDEO_CODEC_FORMAT_MJPEG : VIDEO_CODEC_FORMAT_H264;
		mVideoInfo.nWidth = mFrameWidth;;
		mVideoInfo.nHeight = mFrameHeight;
		mVideoInfo.nFrameRate = mFrameRate;
		mVideoInfo.nFrameDuration = 1000*1000/mFrameRate;
		mVideoInfo.nAspectRatio = 1000;
		mVideoInfo.bIs3DStream = 0;
		mVideoInfo.nCodecSpecificDataLen = 0;
		mVideoInfo.pCodecSpecificData = NULL;

		Libve_init2(&mDecoder,&mVideoInfo,&mVideoConf);
		if(mDecoder == NULL){
			LOGE("FUNC:%s, Line:%d ",__FUNCTION__,__LINE__);
		}
	}

#endif
#ifdef CDX_FOR_PURE_H264

	if(((mCaptureFormat == V4L2_PIX_FMT_H264) && (SONIX_CAMERA_H264 == 1)))
	{
		LOGD("FUNC:%s, Line:%d init Dec!",__FUNCTION__,__LINE__);
		//* all decoder support YV12 format.
		mVideoConf.eOutputPixelFormat  = PIXEL_FORMAT_NV21;//PIXEL_FORMAT_YV12;PIXEL_FORMAT_YUV_MB32_420;//
		//* never decode two picture when decoding a thumbnail picture.
		mVideoConf.bDisable3D		  = 1;

		mVideoConf.nVbvBufferSize	  = 0;
		mVideoConf.memops = MemAdapterGetOpsS();

		mVideoInfo.eCodecFormat =(mCaptureFormat == V4L2_PIX_FMT_MJPEG) ? VIDEO_CODEC_FORMAT_MJPEG : VIDEO_CODEC_FORMAT_H264;
		mVideoInfo.nWidth = mFrameWidth;;
		mVideoInfo.nHeight = mFrameHeight;
		mVideoInfo.nFrameRate = mFrameRate;
		mVideoInfo.nFrameDuration = 1000*1000/mFrameRate;
		mVideoInfo.nAspectRatio = 1000;
		mVideoInfo.bIs3DStream = 0;
		mVideoInfo.nCodecSpecificDataLen = 0;
		mVideoInfo.pCodecSpecificData = NULL;

		Libve_init2(&mDecoder,&mVideoInfo,&mVideoConf);
		if(mDecoder == NULL){
			LOGE("FUNC:%s, Line:%d ",__FUNCTION__,__LINE__);
		}
	}

#endif

#ifdef SUPPORT_VIRTUAL_CAMERA
    if (mCameraType == CAMERA_TYPE_CSI) {
        pipe(mCsiFdsC2V);
        mCsiRunning = true;
    }
#endif
    return NO_ERROR;
}

status_t V4L2CameraDevice::stopDevice()
{
	LOGD("V4L2CameraDevice::stopDevice");
	
	pthread_mutex_lock(&mConnectMutex);
	if (!mCanBeDisconnected)
	{
		LOGW("wait until capture thread pause or exit");
		pthread_cond_wait(&mConnectCond, &mConnectMutex);
	}
	pthread_mutex_unlock(&mConnectMutex);
	
	Mutex::Autolock locker(&mObjectLock);
	
	if (!isStarted()) 
	{
		LOGW("%s: camera device is not started.", __FUNCTION__);
		return NO_ERROR;
	}
#ifdef SUPPORT_VIRTUAL_CAMERA
    if (mCameraType == CAMERA_TYPE_VIR && mCsiRunning) {
        mCameraDeviceState = STATE_CONNECTED;
        mCsiVirRunning = false;
        return NO_ERROR;
    }
#endif

	// v4l2 device stop stream
	v4l2StopStreaming();
	
	// v4l2 device unmap buffers
    v4l2UnmapBuf();
	
	for(int i = 0; i < NB_BUFFER; i++)
	{
		memset(&mV4l2buf[i], 0, sizeof(V4L2BUF_t));
	}
	
	mCameraDeviceState = STATE_CONNECTED;

	mLastZoom = -1;
	
	mCurrentV4l2buf = NULL;
#ifdef ADAS_ENABLE
	mCurV4l2Buf = NULL;
#endif

#ifdef CDX10
    if(mCaptureFormat == V4L2_PIX_FMT_MJPEG)
    {
        Libve_exit(&mDecoder,&mDecoderMutex);
    }
#endif
#ifdef CDX20
	if(mCaptureFormat == V4L2_PIX_FMT_MJPEG)
	{
		Libve_exit2(&mDecoder);
	}
#endif
#ifdef CDX_FOR_PURE_H264
	if(mCaptureFormat == V4L2_PIX_FMT_H264)
	{
		Libve_exit2(&mDecoder);
	}
#endif

#ifdef SUPPORT_VIRTUAL_CAMERA
    if (mCameraType == CAMERA_TYPE_CSI) {
        mCsiRunning = false;
        close(mCsiFdsC2V[0]);
        close(mCsiFdsC2V[1]);
    }
#endif
    return NO_ERROR;
}

status_t V4L2CameraDevice::startDeliveringFrames()
{
	F_LOG;
	
	pthread_mutex_lock(&mCaptureMutex);

	if (mCaptureThreadState == CAPTURE_STATE_NULL)
	{
		LOGE("error state of capture thread, %s", __FUNCTION__);
		pthread_mutex_unlock(&mCaptureMutex);
		return EINVAL;
	}

	if (mCaptureThreadState == CAPTURE_STATE_STARTED)
	{
		LOGW("capture thread has already started");
		pthread_mutex_unlock(&mCaptureMutex);
		return NO_ERROR;
	}

	// singal to start capture thread
	mCaptureThreadState = CAPTURE_STATE_STARTED;
	pthread_cond_signal(&mCaptureCond);
	pthread_mutex_unlock(&mCaptureMutex);

	return NO_ERROR;
}

status_t V4L2CameraDevice::stopDeliveringFrames()
{
	F_LOG;
	
	pthread_mutex_lock(&mCaptureMutex);
	if (mCaptureThreadState == CAPTURE_STATE_NULL)
	{
		LOGE("error state of capture thread, %s", __FUNCTION__);
		pthread_mutex_unlock(&mCaptureMutex);
		return EINVAL;
	}

	if (mCaptureThreadState == CAPTURE_STATE_PAUSED)
	{
		LOGW("capture thread has already paused");
		pthread_mutex_unlock(&mCaptureMutex);
		return NO_ERROR;
	}

	mCaptureThreadState = CAPTURE_STATE_PAUSED;
	pthread_mutex_unlock(&mCaptureMutex);

	return NO_ERROR;
}


/****************************************************************************
 * Worker thread management.
 ***************************************************************************/

int V4L2CameraDevice::v4l2WaitCameraReady()
{
	fd_set fds;		
	struct timeval tv;
	int r;

	FD_ZERO(&fds);
	FD_SET(mCameraFd, &fds);		
	
	/* Timeout */
	tv.tv_sec  = 2;
	tv.tv_usec = 0;
	
	r = select(mCameraFd + 1, &fds, NULL, NULL, &tv);
	if (r == -1) 
	{
		LOGE("select err, %s", strerror(errno));
		return -1;
	} 
	else if (r == 0) 
	{
		LOGE("select timeout %s",mHalCameraInfo.device_name);
		int i;
		V4L2BUF_t      *p= NULL;
		for(i=0;i<NB_BUFFER;i++)
		{
			p=  &mV4l2buf[i];
			if(p->refCnt>0){
				LOGE("select timeout force %d",p->index);
				releasePreviewFrame(p->index);
			}
		}
		
		return -1;
	}

	return 0;
}

void V4L2CameraDevice::singalDisconnect()
{
	pthread_mutex_lock(&mConnectMutex);
	mCanBeDisconnected = true;
	pthread_cond_signal(&mConnectCond);
	pthread_mutex_unlock(&mConnectMutex);
}

int V4L2CameraDevice::testFrameRate()
{
    mPicCnt++;

    if(mPicCnt>=1500)
    {
        timeval cur_time;
        gettimeofday(&cur_time, NULL);
        const uint64_t now_time = cur_time.tv_sec * 1000000LL + cur_time.tv_usec;
        int framerate = mPicCnt*1000/((now_time - mOrgTime)/1000);
        if(framerate){
            if(mCameraType == CAMERA_TYPE_UVC){
                LOGD("Mic UVC framerate = %dfps@%dx%d ",framerate,mFrameWidth, mFrameHeight);
            }else if (mCameraType == CAMERA_TYPE_CSI){
                LOGD("Mic CSI framerate = %dfps@%dx%d ",framerate,mFrameWidth, mFrameHeight);
            }else{
                LOGD("Mic TVIN framerate = %dfps@%dx%d ",framerate,mFrameWidth, mFrameHeight);
            }
        }
        mOrgTime = now_time;
        mPicCnt = 0;
    }
    return 0;
}
#ifdef WATERMARK
static void getTimeString(char *str, int len)
{
    time_t timep;
    struct tm *p;
    int year, month, day, hour, min, sec;

    time(&timep);
    p = localtime(&timep);
    year = p->tm_year + 1900;
    month = p->tm_mon + 1;
    day = p->tm_mday;
    hour = p->tm_hour;
    min = p->tm_min;
    sec = p->tm_sec;

	   
#ifndef WATERMARK_USE_MS
	snprintf(str, len, "%d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, min, sec);
#else
	
	struct timeval t;
	t.tv_sec = t.tv_usec = 0;
	gettimeofday(&t, NULL);

    snprintf(str, len, "%d-%02d-%02d %02d:%02d:%02d.%02d", year, month, day, hour, min, sec,t.tv_usec/10000);
#endif
}
void V4L2CameraDevice::waterMarkinit()
{
	if (mWaterMarkEnable) {
		ALOGW("Water mark is already running!");
		return;
	}
	ALOGI("<waterMarkinit>");
    if(mFrameHeight > 1080){
        mWaterMarkCtrlRec = initialwaterMark(1080);
    }else{
        mWaterMarkCtrlRec = initialwaterMark(mFrameHeight);
    }
    if (NULL == mWaterMarkCtrlRec)
	{
		LOGE("Fail to initialize record water mark!");
		return;
	}
	mWaterMarkCtrlPrev = initialwaterMark(540);
	if (NULL == mWaterMarkCtrlPrev)
	{
		LOGE("Fail to initialize preview water mark!");
		return;
	}
	mWaterMarkMultiple = initWaterMarkMultiple();

	mWaterMarkEnable = true;
}

void V4L2CameraDevice::waterMarkDestroy()
{
	if (!mWaterMarkEnable) {
		ALOGW("Water mark is not running yet!");
		return;
	}
	ALOGI("<waterMarkDestroy>");
	pthread_mutex_lock(&mWaterMarkLock);
	mWaterMarkEnable = false;
	if (mWaterMarkCtrlRec != NULL) {
		releaseWaterMark(mWaterMarkCtrlRec);
		mWaterMarkCtrlRec = NULL;
	}
	if (mWaterMarkCtrlPrev != NULL) {
		releaseWaterMark(mWaterMarkCtrlPrev);
		mWaterMarkCtrlPrev = NULL;
	}

	if (mWaterMarkMultiple != NULL) {
		releaseWaterMarkMultiple(mWaterMarkMultiple);
		mWaterMarkMultiple = NULL;
	}

	pthread_mutex_unlock(&mWaterMarkLock);
}

void V4L2CameraDevice::setWaterMarkDispMode(int dispMode)
{
    ALOGI("F:%s,L:%d,dispMode:%d",__FUNCTION__,__LINE__,dispMode);
    mWaterMarkDispMode = dispMode;
}

void V4L2CameraDevice::addWaterMark(unsigned char *addrVirY,int width, int height)
{
    pthread_mutex_lock(&mWaterMarkLock);
    if (mWaterMarkEnable && mWaterMarkCtrlRec != NULL && mWaterMarkMultiple != NULL) {
        mWaterMarkIndata.y = addrVirY;
        mWaterMarkIndata.c = addrVirY + width * height;
        mWaterMarkIndata.width = width;
        mWaterMarkIndata.height = height;
        mWaterMarkIndata.resolution_rate = (float)mThumbHeight/(float)mFrameHeight;

        char time_watermark[32];
        getTimeString(time_watermark,32);

        char buffer_rec[128];
        mCallbackNotifier->getWaterMarkMultiple(buffer_rec);
        //LOGW("buffer_rec = %s",buffer_rec);
        doWaterMarkMultiple(&mWaterMarkIndata, mWaterMarkCtrlRec, mWaterMarkMultiple, buffer_rec, time_watermark);
#ifdef USE_CDX_NEW_MEM_API
CdcMemFlushCache(mCameraMemops,(void*)addrVirY, width * height*3/2);

#else
        ion_flush_cache((void*)addrVirY, width * height*3/2);
#endif
    }
    pthread_mutex_unlock(&mWaterMarkLock);

}
#endif
bool V4L2CameraDevice::captureThread()
{
	static int frame_count = 0;

	pthread_mutex_lock(&mCaptureMutex);
	// stop capture
	if (mCaptureThreadState == CAPTURE_STATE_PAUSED)
	{
		singalDisconnect();
		// wait for signal of starting to capture a frame
		LOGV("capture thread paused");
		pthread_cond_wait(&mCaptureCond, &mCaptureMutex);
	}

	// thread exit
	if (mCaptureThreadState == CAPTURE_STATE_EXIT)
	{
		singalDisconnect();
		LOGV("capture thread exit");
		pthread_mutex_unlock(&mCaptureMutex);
		return false;
	}
	pthread_mutex_unlock(&mCaptureMutex);

    int ret = -1;
    V4L2BUF_t v4l2_buf;

#ifdef SUPPORT_VIRTUAL_CAMERA
    if (mCameraType == CAMERA_TYPE_VIR && mCsiRunning) {
        ret = receiveDataFromCsiCamera(&v4l2_buf);
    } else
#endif
	{
        ret = v4l2WaitCameraReady();
    }


	pthread_mutex_lock(&mCaptureMutex);
	// stop capture or thread exit
	if (mCaptureThreadState == CAPTURE_STATE_PAUSED
		|| mCaptureThreadState == CAPTURE_STATE_EXIT)
	{
		singalDisconnect();
		LOGW("should stop capture now");
		pthread_mutex_unlock(&mCaptureMutex);
		return __LINE__;
	}

	if (ret != 0)
	{
		LOGW("wait v4l2 buffer time out %s",mHalCameraInfo.device_name);
		pthread_mutex_unlock(&mCaptureMutex);

		LOGW("preview queue has %d items.", OSAL_GetElemNum(&mQueueBufferPreview));
		return __LINE__;
	}

#ifdef SUPPORT_VIRTUAL_CAMERA
    if (mCameraType == CAMERA_TYPE_VIR && mCsiRunning) {
        memcpy(&mV4l2buf[v4l2_buf.index], &v4l2_buf, sizeof(V4L2BUF_t));
        ret = OSAL_Queue(&mQueueBufferPreview, &mV4l2buf[v4l2_buf.index]);
        if (ret == 0) {
            pthread_cond_signal(&mPreviewCond);
        }
        pthread_mutex_unlock(&mCaptureMutex);
        return true;
    }
#endif
	// get one video frame
	struct v4l2_buffer buf;
	memset(&buf, 0, sizeof(v4l2_buffer));
	ret = getPreviewFrame(&buf);
	if (ret != OK)
	{	
		pthread_mutex_unlock(&mCaptureMutex);
		usleep(10000);
		return ret;
	}else
	{
		//ALOGV("getPreviewFrame len =%d",buf.bytesused);
	}

	mCurAvailBufferCnt--;

	if (mCurAvailBufferCnt <= NB_BUFFER/2)
	{
		ALOGE("CID %s mCurAvailBufferCnt =%d",mHalCameraInfo.device_name,mCurAvailBufferCnt );
		//mNeedHalfFrameRate = true;
		mStatisicsIndex = 0;
	}
	else if (mNeedHalfFrameRate)
	{
		mStatisicsIndex++;
		if (mStatisicsIndex >= STATISICS_CNT)
		{
			mNeedHalfFrameRate = false;
		}
	}
	
	// deal with this frame
	//mCurFrameTimestamp = (int64_t)((int64_t)buf.timestamp.tv_usec + (((int64_t)buf.timestamp.tv_sec) * 1000000));

	mCurFrameTimestamp = (int64_t)systemTime();

	if (mLastZoom != mNewZoom)
	{
		float widthRate = (float)mFrameWidth / (float)mVideoWidth;
		float heightRate = (float)mFrameHeight / (float)mVideoHeight;
		if (mIsThumbUsedForVideo && (widthRate > 4.0) && (heightRate > 4.0))
		{
			calculateCrop(&mRectCrop, mNewZoom, mMaxZoom, mFrameWidth/2, mFrameHeight/2);	// for cts, to do 
		}
		else
		{
			// the main frame crop
			calculateCrop(&mRectCrop, mNewZoom, mMaxZoom, mFrameWidth, mFrameHeight);
		}
		mCameraHardware->setNewCrop(&mRectCrop);
		
		// the sub-frame crop
		if (mHalCameraInfo.fast_picture_mode)
		{
			calculateCrop(&mThumbRectCrop, mNewZoom, mMaxZoom, mThumbWidth, mThumbHeight);
		}
		
		mLastZoom = mNewZoom;
		if(mCameraType == CAMERA_TYPE_TVIN_NTSC){
			mThumbRectCrop.left = 0;
			mThumbRectCrop.top = 0;
			mThumbRectCrop.right = 719;
			mThumbRectCrop.bottom = 479;
		}
		if(mCameraType == CAMERA_TYPE_TVIN_PAL){
			mThumbRectCrop.left = 0;
			mThumbRectCrop.top = 0;
			mThumbRectCrop.right = 719;
			mThumbRectCrop.bottom = 575;
		}
		LOGV("CROP: [%d, %d, %d, %d]", mRectCrop.left, mRectCrop.top, mRectCrop.right, mRectCrop.bottom);
		LOGV("thumb CROP: [%d, %d, %d, %d]", mThumbRectCrop.left, mThumbRectCrop.top, mThumbRectCrop.right, mThumbRectCrop.bottom);
	}

	if (mVideoFormat != V4L2_PIX_FMT_YUYV
		&& mCaptureFormat == V4L2_PIX_FMT_YUYV)
	{
#if 0//def USE_MP_CONVERT
		YUYVToYUV420C((void*)buf.m.offset, 
					  (void*)(mVideoBuffer.buf_phy_addr[buf.index] | 0x40000000),
					  mFrameWidth, 
					  mFrameHeight);
#else
#ifdef USE_CDX_NEW_MEM_API
		
		CdcMemFlushCache(mCameraMemops,(void*)mVideoBuffer.buf_vir_addr[buf.index], mFrameWidth * mFrameHeight *3/2);

		YUYVToNV21(mMapMem.mem[buf.index], 
					   (void*)mVideoBuffer.buf_vir_addr[buf.index], 
					   mFrameWidth, 
					   mFrameHeight);
		CdcMemFlushCache(mCameraMemops,(void*)mVideoBuffer.buf_vir_addr[buf.index], mFrameWidth * mFrameHeight *3/2);	
#else

		ion_flush_cache((void*)mVideoBuffer.buf_vir_addr[buf.index], mFrameWidth * mFrameHeight *3/2);

		YUYVToNV21(mMapMem.mem[buf.index], 
					   (void*)mVideoBuffer.buf_vir_addr[buf.index], 
					   mFrameWidth, 
					   mFrameHeight);
		ion_flush_cache((void*)mVideoBuffer.buf_vir_addr[buf.index], mFrameWidth * mFrameHeight *3/2);
#endif
#endif
	}

	// V4L2BUF_t for preview and HW encoder
    //V4L2BUF_t v4l2_buf;
	if (mVideoFormat != V4L2_PIX_FMT_YUYV
		&& mCaptureFormat == V4L2_PIX_FMT_YUYV)
	{
		v4l2_buf.addrPhyY		= mVideoBuffer.buf_phy_addr[buf.index] ;//- 0x20000000; //& 0x0fffffff;
		v4l2_buf.addrVirY		= mVideoBuffer.buf_vir_addr[buf.index];
		v4l2_buf.width			= mFrameWidth;
		v4l2_buf.height			= mFrameHeight;
	}

	else
	{
		v4l2_buf.addrPhyY		= buf.m.offset;// - 0x20000000;  //dram addr offset for ve 

#ifdef __SUN9I__
		v4l2_buf.addrPhyY		= buf.m.offset;  //dram addr offset for ve
#endif
		v4l2_buf.addrVirY		= (unsigned int)mMapMem.mem[buf.index];
		v4l2_buf.width			= mFrameWidth;
		v4l2_buf.height			= mFrameHeight;
	}
	#ifdef VIDEO1_MIRROR
	//for process image
		if(strstr(mHalCameraInfo.device_name,"video1")){
			//ALOGV("-----------flip video1--------");
			//if(mCameraMemops!=NULL)
			//	CdcMemFlushCache(mCameraMemops,(void*)v4l2_buf.addrVirY,v4l2_buf.width * v4l2_buf.height * 3 / 2);
			g2d_ops(1,(unsigned char *)v4l2_buf.addrPhyY, v4l2_buf.width, v4l2_buf.height, (unsigned char *)mG2dAddrPhy, v4l2_buf.width,v4l2_buf.height);
			if(mCameraMemops!=NULL)
				CdcMemFlushCache(mCameraMemops,(void*)mG2dAddrVir,v4l2_buf.width * v4l2_buf.height * 3 / 2);
	
			memcpy((void*)v4l2_buf.addrVirY,( const void *)mG2dAddrVir,v4l2_buf.height*v4l2_buf.width*3/2);
			if(mCameraMemops!=NULL)
				CdcMemFlushCache(mCameraMemops,(void*)v4l2_buf.addrVirY,v4l2_buf.width * v4l2_buf.height * 3 / 2);
		}
		#endif
#ifdef WATERMARK
    if(mWaterMarkDispMode == WATER_MARK_DISP_MODE_VIDEO_AND_PREVIEW){
        addWaterMark((unsigned char *)v4l2_buf.addrVirY, v4l2_buf.width, v4l2_buf.height);
    }
#endif
	v4l2_buf.index				= buf.index;
	v4l2_buf.timeStamp			= mCurFrameTimestamp;
	//ALOGE("----------orgpts=%lld",mCurFrameTimestamp);
	v4l2_buf.crop_rect.left		= mRectCrop.left;
	v4l2_buf.crop_rect.top		= mRectCrop.top;
	v4l2_buf.crop_rect.width	= mRectCrop.right - mRectCrop.left + 1;
	v4l2_buf.crop_rect.height	= mRectCrop.bottom - mRectCrop.top + 1;
	v4l2_buf.format				= mVideoFormat;

	v4l2_buf.bytesused	        = buf.bytesused;
	if (mHalCameraInfo.fast_picture_mode)
	{
		v4l2_buf.isThumbAvailable		= 1;
		v4l2_buf.thumbUsedForPreview	= 1;
		v4l2_buf.thumbUsedForPhoto		= 0;
		if(mIsThumbUsedForVideo == true)
		{
			v4l2_buf.thumbUsedForVideo		= 1;
		}
		else
		{
			v4l2_buf.thumbUsedForVideo		= 0;
		}
		v4l2_buf.thumbAddrPhyY			= v4l2_buf.addrPhyY + ALIGN_4K(ALIGN_32B(mFrameWidth) * mFrameHeight * 3 / 2);	// to do
		v4l2_buf.thumbAddrVirY			= v4l2_buf.addrVirY + ALIGN_4K(ALIGN_32B(mFrameWidth) * mFrameHeight * 3 / 2);	// to do
		v4l2_buf.thumbWidth				= mThumbWidth;
		v4l2_buf.thumbHeight			= mThumbHeight;
		v4l2_buf.thumb_crop_rect.left	= mThumbRectCrop.left;
		v4l2_buf.thumb_crop_rect.top	= mThumbRectCrop.top;
		v4l2_buf.thumb_crop_rect.width	= mThumbRectCrop.right - mThumbRectCrop.left;
		v4l2_buf.thumb_crop_rect.height	= mThumbRectCrop.bottom - mThumbRectCrop.top;
		v4l2_buf.thumbFormat			= mVideoFormat;
#ifdef WATERMARK
        if(mWaterMarkDispMode == WATER_MARK_DISP_MODE_VIDEO_AND_PREVIEW){
            addWaterMark((unsigned char *)v4l2_buf.thumbAddrVirY, v4l2_buf.thumbWidth, v4l2_buf.thumbHeight);
        }
#endif
	}
	else
	{
		v4l2_buf.isThumbAvailable		= 0;
	}
/*
    int SnrValue = getSnrValue();
    int SnrLevel = SnrValue >> 8;
    int Gain = (SnrValue & 0xff);
    
    if (SnrLevel != 0 && Gain > 16)
    {
        Gain = Gain/16;
	    char *cnr_uv = (char *)v4l2_buf.thumbAddrVirY + ALIGN_16B(v4l2_buf.thumbWidth) * v4l2_buf.thumbHeight;
		PreviewCnr(SnrLevel, Gain, v4l2_buf.thumbWidth, v4l2_buf.thumbHeight,cnr_uv , cnr_uv);
		if(mTakePictureState == TAKE_PICTURE_FAST)
	    {
			cnr_uv = (char *)v4l2_buf.addrVirY + ALIGN_16B(mFrameWidth) * mFrameHeight;
			PreviewCnr(SnrLevel+1, Gain, mFrameWidth, mFrameHeight,cnr_uv , cnr_uv);
        }
    }
*/

	v4l2_buf.refCnt = 1;
	memcpy(&mV4l2buf[v4l2_buf.index], &v4l2_buf, sizeof(V4L2BUF_t));
#if 1
//def CAMERA_MANAGER_ENABLE
	if((mCameraManager != NULL) && mCameraManager->isOviewEnable())
	{
		mCameraManager->queueCameraBuf(mHalCameraInfo.device_id,&v4l2_buf);
		goto DEC_REF;
	}
#endif
	if ((!mVideoHint) && (mTakePictureState != TAKE_PICTURE_NORMAL))
	{
		// face detection only use when picture mode
		mCurrentV4l2buf = &mV4l2buf[v4l2_buf.index];
	}

#ifdef SUPPORT_VIRTUAL_CAMERA
    if (mCameraType == CAMERA_TYPE_CSI && mCsiVirRunning) {
        sendDataToVirCamera(&v4l2_buf);
    }
#endif

	#ifdef USE_Deinterlace_HW
	//if (mCameraType == CAMERA_TYPE_TVIN_NTSC||mCameraType == CAMERA_TYPE_TVIN_PAL||mCameraType == CAMERA_TYPE_TVIN_YPBPR)
	if (mCameraType == CAMERA_TYPE_TVIN_NTSC||mCameraType == CAMERA_TYPE_TVIN_PAL)
	{
		unsigned int phy_addr = mdiBuffer_pre.addrPhyY;
		unsigned int vir_addr = mdiBuffer_pre.addrVirY;
		int frame_size = mFrameWidth * mFrameHeight * 3 >> 1;
		V4L2BUF_t * pbuf = (V4L2BUF_t *)OSAL_Dequeue(&mQueueBufferDi);

		if(di_select==1){
				Hw_Di_Process((unsigned char*)pbuf->addrPhyY,mFrameWidth,mFrameHeight,
							(unsigned char *)v4l2_buf.addrPhyY,mFrameWidth,mFrameHeight,
							(unsigned char *)mdiBuffer.addrPhyY,mFrameWidth,mFrameHeight,0);
				usleep(3); 
				Hw_Di_Process((unsigned char*)pbuf->addrPhyY,mFrameWidth,mFrameHeight,(unsigned char *)v4l2_buf.addrPhyY,
				mFrameWidth,mFrameHeight,(unsigned char *)mdiBuffer.addrPhyY,mFrameWidth,mFrameHeight,1);
			    memcpy((void*)&mdiBuffer_pre, &v4l2_buf, sizeof(V4L2BUF_t));	// save preinfb frame	
			    mdiBuffer_pre.addrPhyY = phy_addr;
				mdiBuffer_pre.addrVirY = vir_addr;
			#ifdef USE_CDX_NEW_MEM_API
				CdcMemFlushCache(mCameraMemops,(void*)v4l2_buf.addrVirY, frame_size);
			#else
			    ion_flush_cache((void*)v4l2_buf.addrVirY, frame_size);
			#endif	
			
				memcpy((void*)mdiBuffer_pre.addrVirY, (void*)v4l2_buf.addrVirY, frame_size);

			#ifdef USE_CDX_NEW_MEM_API
				CdcMemFlushCache(mCameraMemops,(void*)mdiBuffer_pre.addrVirY, mFrameWidth*mFrameHeight*3/2);
			#else
		  		ion_flush_cache((void*)mdiBuffer_pre.addrVirY, mFrameWidth*mFrameHeight*3/2);	  //copy to infb buffer
			#endif
				ret = OSAL_Queue(&mQueueBufferDi, &mdiBuffer_pre);
		        mV4l2buf[v4l2_buf.index].addrVirY=mdiBuffer.addrVirY;}
		else
		{
			di_select=1;
			memcpy((void*)&mdiBuffer_pre, &v4l2_buf, sizeof(V4L2BUF_t));	// save preinfb frame	
			mdiBuffer_pre.addrPhyY = phy_addr;
			mdiBuffer_pre.addrVirY = vir_addr;
		#ifdef USE_CDX_NEW_MEM_API
			CdcMemFlushCache(mCameraMemops,(void*)v4l2_buf.addrVirY, frame_size);
		#else
		    ion_flush_cache((void*)v4l2_buf.addrVirY, frame_size);
		#endif
			memcpy((void*)mdiBuffer_pre.addrVirY, (void*)v4l2_buf.addrVirY, frame_size);

		#ifdef USE_CDX_NEW_MEM_API
			CdcMemFlushCache(mCameraMemops,(void*)mdiBuffer_pre.addrVirY, mFrameWidth*mFrameHeight*3/2);
		#else
	  		ion_flush_cache((void*)mdiBuffer_pre.addrVirY, mFrameWidth*mFrameHeight*3/2);	  //copy to infb buffer
		#endif
			ret = OSAL_Queue(&mQueueBufferDi, &mdiBuffer_pre);
			if (ret != 0)
			{
				LOGE("di queue full");
				
			}
				}

	
	}
	#endif
	if (mTakePictureState == TAKE_PICTURE_NORMAL)
	{
		//copy picture buffer
		unsigned int phy_addr = mPicBuffer.addrPhyY;
		unsigned int vir_addr = mPicBuffer.addrVirY;
		int frame_size = mFrameWidth * mFrameHeight * 3 >> 1;

		if (frame_size > MAX_PICTURE_SIZE)
		{
			LOGE("picture buffer size(%d) is smaller than the frame buffer size(%d)", MAX_PICTURE_SIZE, frame_size);
			pthread_mutex_unlock(&mCaptureMutex);
			return false;
		}
#ifdef CDX_FOR_PURE_H264
			if(((mCaptureFormat == V4L2_PIX_FMT_H264) && (SONIX_CAMERA_H264 == 1)))
			{
				
				if(mDecoder == NULL){
					LOGE("mDecoder==NULL,init again");
					Libve_init2(&mDecoder,&mVideoInfo,&mVideoConf);
				}
				mDataInfo.nLength		=buf.bytesused;
				mDataInfo.nPts			=(int64_t)mCurFrameTimestamp/1000;
	#ifdef USE_CDX_NEW_MEM_API
				CdcMemFlushCache(mCameraMemops,(void*)mPicBuffer.addrVirY, mFrameWidth*mFrameHeight*3/2);
	#else
				ion_flush_cache((void*)mPicBuffer.addrVirY, mFrameWidth*mFrameHeight*3/2);
		
	#endif
				Libve_dec2(&mDecoder,
					mMapMem.mem[buf.index],
					(void*)mPicBuffer.addrVirY,
					&mVideoInfo,
					&mDataInfo,
					&mVideoConf,mCameraMemops);
		
		
	#ifdef USE_CDX_NEW_MEM_API	
				CdcMemFlushCache(mCameraMemops,(void*)mPicBuffer.addrVirY, mFrameWidth*mFrameHeight*3/2);
	#else
				ion_flush_cache((void*)mPicBuffer.addrVirY, mFrameWidth*mFrameHeight*3/2);
	#endif
	
			}else
#endif
		{
			memcpy((void*)&mPicBuffer, &v4l2_buf, sizeof(V4L2BUF_t));
			mPicBuffer.addrPhyY = phy_addr;
			mPicBuffer.addrVirY = vir_addr;

#ifdef USE_ION_MEM_ALLOCATOR
			ion_flush_cache((void*)v4l2_buf.addrVirY, frame_size);
			memcpy((void*)mPicBuffer.addrVirY, (void*)v4l2_buf.addrVirY, frame_size);
			ion_flush_cache((void*)mPicBuffer.addrVirY, frame_size);
#elif USE_CDX_NEW_MEM_API
			CdcMemFlushCache(mCameraMemops,(void*)v4l2_buf.addrVirY, frame_size);
			memcpy((void*)mPicBuffer.addrVirY, (void*)v4l2_buf.addrVirY, frame_size);
			CdcMemFlushCache(mCameraMemops,(void*)mPicBuffer.addrVirY, frame_size);
#else USE_SUNXI_MEM_ALLOCATOR
			sunxi_flush_cache((void*)v4l2_buf.addrVirY, frame_size);
			memcpy((void*)mPicBuffer.addrVirY, (void*)v4l2_buf.addrVirY, frame_size);
			sunxi_flush_cache((void*)mPicBuffer.addrVirY, frame_size);
#endif


	}

		// enqueue picture buffer
		ret = OSAL_Queue(&mQueueBufferPicture, &mPicBuffer);
		if (ret != 0)
		{
			LOGW("picture queue full");
			pthread_cond_signal(&mPictureCond);
			goto DEC_REF;
		}
		
		mIsPicCopy = true;
		mCaptureThreadState = CAPTURE_STATE_PAUSED;
		mTakePictureState = TAKE_PICTURE_NULL;
		pthread_cond_signal(&mPictureCond);
		
		goto DEC_REF;
	}
	else
	{
		ret = OSAL_Queue(&mQueueBufferPreview, &mV4l2buf[v4l2_buf.index]);
		if (ret != 0)
		{
			LOGW("preview queue full");
			goto DEC_REF;
		}
		//F_LOG

		// add reference count
		mV4l2buf[v4l2_buf.index].refCnt++;
		//ALOGD("capture thread 2028 ID %d refCnt %d",v4l2_buf.index, mV4l2buf[v4l2_buf.index].refCnt);
		// signal a new frame for preview
		pthread_cond_signal(&mPreviewCond);
		if(mCurAvailBufferCnt<NB_BUFFER/2)
			ALOGD("capture thread 2028 v4l2 buf ID %d refCnt %d",v4l2_buf.index, mV4l2buf[v4l2_buf.index].refCnt);
		#if 0
		{
			static int cntt=0;
			cntt++;
			char buf[256];
			if(cntt<150){
				LOGE("save h264 date-----p=%p----len=%d-----------",v4l2_buf.addrVirY,v4l2_buf.bytesused);
				//cntt=0;
				sprintf(buf,"/tmp/b.h264");
				saveframe(buf,(void*)v4l2_buf.addrVirY,v4l2_buf.bytesused,0);
			}

		}
		#endif
		if (mTakePictureState == TAKE_PICTURE_FAST
			|| mTakePictureState == TAKE_PICTURE_RECORD)
		{
			if (mHalCameraInfo.fast_picture_mode)
			{
				if (buf.reserved == 0xFFFFFFFF)
				{
					goto DEC_REF;
				}
			}
			
			// enqueue picture buffer
			ret = OSAL_Queue(&mQueueBufferPicture, &mV4l2buf[v4l2_buf.index]);
			if (ret != 0)
			{
				LOGW("picture queue full");
				pthread_cond_signal(&mPictureCond);
				goto DEC_REF;
			}
			
			// add reference count
			mV4l2buf[v4l2_buf.index].refCnt++;
			
			ALOGD("capture thread 2068 ID %d refCnt %d",v4l2_buf.index, mV4l2buf[v4l2_buf.index].refCnt);
			mTakePictureState = TAKE_PICTURE_NULL;
			mIsPicCopy = false;
			pthread_cond_signal(&mPictureCond);
		}

		if (mTakePictureState == TAKE_PICTURE_SMART)
		{
			// enqueue picture buffer
			ret = OSAL_Queue(&mQueueBufferPicture, &mV4l2buf[v4l2_buf.index]);
			if (ret != 0)
			{
				LOGW("picture queue full");
				pthread_cond_signal(&mSmartPictureCond);
				goto DEC_REF;
			}
			
			// add reference count
			mV4l2buf[v4l2_buf.index].refCnt++;
			
			ALOGD("capture thread 2088 ID %d refCnt %d",v4l2_buf.index, mV4l2buf[v4l2_buf.index].refCnt);
			//mTakePictureState = TAKE_PICTURE_NULL;
			mIsPicCopy = false;
			pthread_cond_signal(&mSmartPictureCond);
		}
		
		if ((mTakePictureState == TAKE_PICTURE_CONTINUOUS
			|| mTakePictureState == TAKE_PICTURE_CONTINUOUS_FAST)
			&& isContinuousPictureTime())
		{
			// enqueue picture buffer
			ret = OSAL_Queue(&mQueueBufferPicture, &mV4l2buf[v4l2_buf.index]);
			if (ret != 0)
			{
				// LOGV("continuous picture queue full");
				pthread_cond_signal(&mContinuousPictureCond);
				goto DEC_REF;
			}

			// add reference count
			mV4l2buf[v4l2_buf.index].refCnt++;
			
			ALOGD("capture thread 2110 cid =%d ID %d refCnt %d",mHalCameraInfo.device_id,v4l2_buf.index, mV4l2buf[v4l2_buf.index].refCnt);
			mIsPicCopy = false;
			pthread_cond_signal(&mContinuousPictureCond);
		}
	}

DEC_REF:
	pthread_mutex_unlock(&mCaptureMutex);
	
	//ALOGD("capture thread 2119 cid =%d ID %d refCnt %d",mHalCameraInfo.device_id,v4l2_buf.index, mV4l2buf[v4l2_buf.index].refCnt);
	releasePreviewFrame(v4l2_buf.index);
	
    return true;
}

#ifdef USE_MP_CONVERT
int V4L2CameraDevice::g2d_scale(unsigned char *src, int src_width, int src_height, unsigned char *dst, int dst_width, int dst_height)
{
	g2d_stretchblt scale;

	scale.flag = G2D_BLT_NONE;//G2D_BLT_NONE;//G2D_BLT_PIXEL_ALPHA|G2D_BLT_ROTATE90;
	scale.src_image.addr[0] = (int)src;
	scale.src_image.addr[1] = (int)src + src_width*src_height;
	scale.src_image.w = src_width;
	scale.src_image.h = src_height;
	scale.src_image.format = G2D_FMT_PYUV420UVC;
	scale.src_image.pixel_seq = G2D_SEQ_NORMAL;
	scale.src_rect.x = 0;
	scale.src_rect.y = 0;
	scale.src_rect.w = src_width;
	scale.src_rect.h = src_height;
	
	scale.dst_image.addr[0] = (int)dst;
	scale.dst_image.addr[1] = (int)dst + dst_width*dst_height;
	//scale.dst_image.addr[2] = (int)dst + dst_width * dst_height * 5 / 4;
	scale.dst_image.w = dst_width;
	scale.dst_image.h = dst_height;
	scale.dst_image.format =G2D_FMT_PYUV420UVC;// G2D_FMT_PYUV420UVC;
	scale.dst_image.pixel_seq = G2D_SEQ_NORMAL;
	scale.dst_rect.x = 0;
	scale.dst_rect.y = 0;
	scale.dst_rect.w = dst_width;
	scale.dst_rect.h = dst_height;
	scale.color = 0xff;
	scale.alpha = 0xff; 

	if(ioctl(mG2DHandle, G2D_CMD_STRETCHBLT, &scale)<0){
		return -1;
	}
	return 0;
}
int V4L2CameraDevice::g2d_ops(int cmd ,unsigned char *src, int src_width, int src_height, unsigned char *dst, int dst_width, int dst_height)
{
	if(cmd==1)
	{
		return g2d_flip(src,src_width, src_height, dst, dst_width, dst_height);
	}
	return 0;
}
int V4L2CameraDevice::g2d_flip(unsigned char *src, int src_width, int src_height, unsigned char *dst, int dst_width, int dst_height)
	{
		g2d_blt 	blit_para;
		int 		err;
		int 		u, v;
		int width  = src_width;
		int height = src_height;

		
		blit_para.src_image.addr[0] 	 = (int)src;
		blit_para.src_image.addr[1] 	 = (int)src + width * height;
		blit_para.src_image.w			 = width;		  /* src buffer width in pixel units */
		blit_para.src_image.h			 = height;		  /* src buffer height in pixel units */
		blit_para.src_image.format		 = G2D_FMT_PYUV420UVC;
		blit_para.src_image.pixel_seq	 = G2D_SEQ_NORMAL;//G2D_SEQ_VUVU;		   /*  */
		blit_para.src_rect.x			 = 0;						/* src rect->x in pixel */
		blit_para.src_rect.y			 = 0;						/* src rect->y in pixel */
		blit_para.src_rect.w			 = width;			/* src rect->w in pixel */
		blit_para.src_rect.h			 = height;			/* src rect->h in pixel */
	
		blit_para.dst_image.addr[0] 	 = (int)dst;							// y
		blit_para.dst_image.addr[1] 	 = (int)dst + width * height;			// uv

		blit_para.dst_image.w			 = width;		  /* dst buffer width in pixel units */ 		
		blit_para.dst_image.h			 = height;		  /* dst buffer height in pixel units */
		blit_para.dst_image.format		 = G2D_FMT_PYUV420UVC;
		blit_para.dst_image.pixel_seq	 = G2D_SEQ_NORMAL;			/* not use now */
		blit_para.dst_x 				 = 0;					/* dst rect->x in pixel */
		blit_para.dst_y 				 = 0;					/* dst rect->y in pixel */
		blit_para.color 				 = 0xff;				/* fix me*/
		blit_para.alpha 				 = 0xff;				/* globe alpha */ 
		
		blit_para.flag = G2D_BLT_FLIP_HORIZONTAL;
		
		err = ioctl(mG2DHandle , G2D_CMD_BITBLT, (unsigned long)&blit_para);				
		if(err < 0)
		{
			LOGE("g2d_flip ioctl, G2D_CMD_BITBLT failed err =%d",err);
			return err;
		}
		return 0;
	}


#endif

bool V4L2CameraDevice::previewThread()
{
	//static int frame_count = 0; //bug for more than two camera,move to class member var

	V4L2BUF_t * pbuf = (V4L2BUF_t *)OSAL_Dequeue(&mQueueBufferPreview);
	if (pbuf == NULL)
	{
		// LOGV("picture queue no buffer, sleep...");
		pthread_mutex_lock(&mPreviewMutex);
		pthread_cond_wait(&mPreviewCond, &mPreviewMutex);
		pthread_mutex_unlock(&mPreviewMutex);
		return true;
	}
	
	if(mCurAvailBufferCnt<NB_BUFFER/2)
		ALOGD("previewThread thread v4l2 [%s] cnt[%d] buf ID %d refCnt %d",mHalCameraInfo.device_name,mCurAvailBufferCnt,pbuf->index, mV4l2buf[pbuf->index].refCnt);
	//F_LOG
#ifdef SUPPORT_VIRTUAL_CAMERA
    if (mCameraType == CAMERA_TYPE_VIR && mCsiRunning) {
        if (pbuf->addrPhyY == 0) {
            LOGW("<F:%s, L:%d> addrPhyY=0!", __FUNCTION__, __LINE__);
            return true;
        }
        mPreviewWindow->onNextFrameAvailable((void*)pbuf,mCameraFlipStatus,0);
        return true;
    }
#endif
	Mutex::Autolock locker(&mObjectLock);
	//fengyun add for h264 recording start
	if (mMapMem.mem[pbuf->index] == NULL
		|| ((pbuf->addrPhyY == 0) && (mCaptureFormat != V4L2_PIX_FMT_H264)))
	{
		LOGW("preview buffer have been released...");
		return true;
	}
	//fengyun add for h264 recording end
	//showformat(pbuf->format,"CallBack");
	// callback
	//LOGV("preview buffer inform  cnt=%d",frame_count);
#ifdef ADAS_ENABLE
	//if ( mAdasStarted)
	//{
		//LOGV("preview buffer inform  cnt=%d camerafd=%d",adasIgnorFrameCnt,mCameraFd);
	//}
	if ( mAdasStarted && ((adasIgnorFrameCnt%2)==0)) {
			g2d_scale((unsigned char *)pbuf->addrPhyY,pbuf->width,pbuf->height,(unsigned char *)mAdasBuffer.addrPhyY,ADAS_WIDTH,ADAS_HEIGHT);
			mCurV4l2Buf = (unsigned char *)mAdasBuffer.addrVirY;
			//LOGV("mAdasStarted buffer inform idx=%d  cnt=%d",pbuf->index,adasIgnorFrameCnt);
			///////mCurV4l2Buf = (unsigned char *)pbuf->addrVirY;
			pthread_mutex_lock(&mAdasMutex);
			pthread_cond_signal(&mAdasCond);
			pthread_mutex_unlock(&mAdasMutex);
	}
	adasIgnorFrameCnt++;
#endif
#ifdef WATERMARK
    if(mWaterMarkDispMode == WATER_MARK_DISP_MODE_VIDEO_ONLY){
        addWaterMark((unsigned char *)pbuf->addrVirY, pbuf->width, pbuf->height);
    }
#endif
	pthread_mutex_lock(&mCaptureMutex);	
	if (mCallbackNotifier->isMessageEnabled(CAMERA_MSG_VIDEO_FRAME) && mCallbackNotifier->isVideoRecordingEnabled()) 
	{//this must uset mutex protect 
		pbuf->refCnt++;
	}
	pthread_mutex_unlock(&mCaptureMutex);

	mCallbackNotifier->onNextFrameAvailable((void*)pbuf, mUseHwEncoder,-1);
	

	// preview
	// if (isPreviewTime())
#ifdef __SUN9I__
	mPreviewWindow->onNextFrameAvailable((void*)pbuf,0,0);
#else
if(mCallbackNotifier->isMessageEnabled(CAMERA_MSG_PREVIEW_FRAME)){

#ifdef USE_MP_CONVERT	
	if((mCameraType == CAMERA_TYPE_CSI) && (mFrameWidth >1920))
	{
#ifdef USE_CDX_NEW_MEM_API
		CdcMemFlushCache(mCameraMemops,(void*)pbuf->addrVirY, mFrameWidth*mFrameHeight*3/2);		
#else	
		ion_flush_cache((void*)pbuf->addrVirY, mFrameWidth*mFrameHeight*3/2);
#endif			
		g2d_scale((unsigned char *)pbuf->addrPhyY,mFrameWidth,mFrameHeight,(unsigned char *)mG2dAddrPhy,SCALE_WIDTH,SCALE_HEIGHT);
		V4L2BUF_t tempBuffer;
		memcpy(&tempBuffer,pbuf,sizeof(V4L2BUF_t));
		tempBuffer.width = SCALE_WIDTH;
		tempBuffer.height = SCALE_HEIGHT;
		tempBuffer.addrVirY = mG2dAddrVir;
		tempBuffer.addrPhyY = mG2dAddrPhy;
		
		mPreviewWindow->onNextFrameAvailable((void*)&tempBuffer,mCameraFlipStatus,0);
	}
	else
#endif
	{
		if (!mNeedHalfFrameRate || mShouldPreview)
		{
//#define CAMERA_NEED_ROTATE 1
#ifdef CAMERA_NEED_ROTATE
#ifdef USE_CDX_NEW_MEM_API
			CdcMemFlushCache(mCameraMemops,(void*)pbuf->addrVirY, mFrameWidth*mFrameHeight*3/2);		
#else	
			ion_flush_cache((void*)pbuf->addrVirY, mFrameWidth*mFrameHeight*3/2);
#endif

			//HwDisplay* hwd=HwDisplay::getInstance();
			//hwd->aut_hwd_imgtr_init();
			//hwd->aut_hwd_imgtr_proc(unsigned int hlay,frm_info_t * imginfo);
			//hwd->aut_hwd_imgtr_release();
			NV21Rotate((const void* )pbuf->addrPhyY, (void *)mG2dAddrPhy, mFrameWidth, mFrameHeight);
			
			V4L2BUF_t tempBuffer;
			memcpy(&tempBuffer,pbuf,sizeof(V4L2BUF_t));
			tempBuffer.addrVirY = mG2dAddrVir;
			tempBuffer.addrPhyY = mG2dAddrPhy;
			tempBuffer.format = V4L2_PIX_FMT_YVU420;
			tempBuffer.width =pbuf->height;
			tempBuffer.height=pbuf->width;
#ifdef USE_CDX_NEW_MEM_API
			CdcMemFlushCache(mCameraMemops,(void*)mG2dAddrVir, mFrameWidth*mFrameHeight*2);		
#else	
			ion_flush_cache((void*)mG2dAddrVir, mFrameWidth*mFrameHeight*2);
#endif			
			mPreviewWindow->onNextFrameAvailable((void*)&tempBuffer,mCameraFlipStatus,0);	
#else //CAMERA_NEED_ROTATE
#if 1
				if((mCaptureFormat == V4L2_PIX_FMT_MJPEG) || (mCaptureFormat == V4L2_PIX_FMT_H264) )
				{
					
					if(mDecoder == NULL){
						LOGE("mDecoder==NULL,init again");
						Libve_init2(&mDecoder,&mVideoInfo,&mVideoConf);
					}
					mDataInfo.nLength		=pbuf->bytesused;
					mDataInfo.nPts			=(int64_t)mCurFrameTimestamp/1000;
	#ifdef USE_CDX_NEW_MEM_API
					CdcMemFlushCache(mCameraMemops,(void*)mVideoBuffer.buf_vir_addr[pbuf->index], mFrameWidth*mFrameHeight*3/2);
	#else
					ion_flush_cache((void*)mVideoBuffer.buf_vir_addr[pbuf->index], mFrameWidth*mFrameHeight*3/2);
			
	#endif
					Libve_dec2(&mDecoder,
						mMapMem.mem[pbuf->index],
						(void*)mVideoBuffer.buf_vir_addr[pbuf->index],
						&mVideoInfo,
						&mDataInfo,
						&mVideoConf,mCameraMemops);
			
			
	#ifdef USE_CDX_NEW_MEM_API	
					CdcMemFlushCache(mCameraMemops,(void*)mVideoBuffer.buf_vir_addr[pbuf->index], mFrameWidth*mFrameHeight*3/2);
	#else
					ion_flush_cache((void*)mVideoBuffer.buf_vir_addr[pbuf->index], mFrameWidth*mFrameHeight*3/2);
			
	#endif

		#if 0
		{
			static int cntt=0;
			cntt++;
			char buf[256];
			if(cntt<20){
				LOGE("save yuv date-----p=%p----len=%d-----------",mVideoBuffer.buf_vir_addr[pbuf->index],mFrameWidth*mFrameHeight*3/2);
				//cntt=0;
				sprintf(buf,"/tmp/%d.yuv",cntt);
				saveframe(buf,(void*)mVideoBuffer.buf_vir_addr[pbuf->index],mFrameWidth*mFrameHeight*3/2,1);
			}

		}
			
		#endif
			V4L2BUF_t tempBuffer;
			memcpy(&tempBuffer,pbuf,sizeof(V4L2BUF_t));
			tempBuffer.addrVirY = mVideoBuffer.buf_vir_addr[pbuf->index];
			tempBuffer.addrPhyY = mVideoBuffer.buf_phy_addr[pbuf->index];;
			tempBuffer.format = V4L2_PIX_FMT_NV21;
			tempBuffer.width =mFrameWidth;
			tempBuffer.height=mFrameHeight;
			tempBuffer.index				= pbuf->index;
			tempBuffer.timeStamp			= mCurFrameTimestamp;
			tempBuffer.crop_rect.left		= pbuf->crop_rect.left;
			tempBuffer.crop_rect.top		= pbuf->crop_rect.top;
			tempBuffer.crop_rect.width	= pbuf->crop_rect.width;
			tempBuffer.crop_rect.height	= pbuf->crop_rect.height;
			tempBuffer.format				= pbuf->format;

			tempBuffer.bytesused	        = pbuf->bytesused;
			mPreviewWindow->onNextFrameAvailable((void*)&tempBuffer,mCameraFlipStatus,0);
	}else
#endif
			CdcMemFlushCache(mCameraMemops,(void*)pbuf->addrVirY, mFrameWidth*mFrameHeight*3/2);
			mPreviewWindow->onNextFrameAvailable((void*)pbuf,mCameraFlipStatus,0);
#endif  //CAMERA_NEED_ROTATE
		}
		mShouldPreview = mShouldPreview ? false : true;
	}
}	
#endif

	//LOGD("preview id : %d cid =%d", pbuf->index,mHalCameraInfo.device_id);

	releasePreviewFrame(pbuf->index);

	return true;
}

// singal picture
bool V4L2CameraDevice::pictureThread()
{
	V4L2BUF_t * pbuf = (V4L2BUF_t *)OSAL_Dequeue(&mQueueBufferPicture);
	if (pbuf == NULL)
	{
		LOGV("picture queue no buffer, sleep...");
		pthread_mutex_lock(&mPictureMutex);
		pthread_cond_wait(&mPictureCond, &mPictureMutex);
		pthread_mutex_unlock(&mPictureMutex);
		return true;
	}

	DBG_TIME_BEGIN("taking picture", 0);
#ifdef WATERMARK
        if(mWaterMarkDispMode == WATER_MARK_DISP_MODE_VIDEO_ONLY){
            addWaterMark((unsigned char *)pbuf->addrVirY, pbuf->width, pbuf->height);
        }
#endif

	// notify picture cb
	mCameraHardware->notifyPictureMsg((void*)pbuf);

	DBG_TIME_DIFF("notifyPictureMsg");

	mCallbackNotifier->takePicture((void*)pbuf);
	
	char str[128];
	sprintf(str, "hw picture size: %dx%d", pbuf->width, pbuf->height);
	DBG_TIME_DIFF(str);
	
	if (!mIsPicCopy)
	{
		releasePreviewFrame(pbuf->index);
	}

	DBG_TIME_END("Take picture", 0);

	return true;
}

// blink picture
bool V4L2CameraDevice::smartPictureThread()
{
	V4L2BUF_t * pbuf = (V4L2BUF_t *)OSAL_Dequeue(&mQueueBufferPicture);

	if (pbuf == NULL)
	{
		LOGV("smartPictureThread queue no buffer, sleep...");
		pthread_mutex_lock(&mSmartPictureMutex);
		pthread_cond_wait(&mSmartPictureCond, &mSmartPictureMutex);
		pthread_mutex_unlock(&mSmartPictureMutex);
		return true;
	}
	
	// apk stop smart pictures
	if (mSmartPictureDone)
	{
		mTakePictureState = TAKE_PICTURE_NULL;
		if (!mIsPicCopy)
		{
			releasePreviewFrame(pbuf->index);
		}
		return true;
	}
	
    #if 0

	ALOGD("!! mCameraHardware->mBlinkPictureResult %d state %d", mCameraHardware->mBlinkPictureResult, mCameraHardware->mBlinkDetectionState);

	if ((mCameraHardware->mBlinkPictureResult == true) && (mCameraHardware->mBlinkDetectionState == FACE_DETECTION_PREPARED)) 
	{
		DBG_TIME_BEGIN("taking blink picture", 0);

		// notify picture cb
		mCameraHardware->notifyPictureMsg((void*)pbuf);

		DBG_TIME_DIFF("notifyPictureMsg");

		mCallbackNotifier->takePicture((void*)pbuf);

		ALOGD("~~ smartPictureThread takePicture");

		stopSmartPicture();
        mTakePictureState = TAKE_PICTURE_NULL;
	}
	
	#endif

	#if 0

	ALOGD("!! mCameraHardware->mSmilePictureResult %d, state %d", mCameraHardware->mSmilePictureResult, mCameraHardware->mSmileDetectionState);

	if ((mCameraHardware->mSmilePictureResult == true) && (mCameraHardware->mSmileDetectionState == FACE_DETECTION_PREPARED))
	{
		DBG_TIME_BEGIN("taking smile picture", 0);

		// notify picture cb
		mCameraHardware->notifyPictureMsg((void*)pbuf);

		DBG_TIME_DIFF("notifyPictureMsg");

		mCallbackNotifier->takePicture((void*)pbuf);

		ALOGD("~~ smartPictureThread takePicture");

		stopSmartPicture();
        mTakePictureState = TAKE_PICTURE_NULL;
	}
	
	#endif

	char str[128];
	sprintf(str, "hw picture size: %dx%d", pbuf->width, pbuf->height);
	DBG_TIME_DIFF(str);
	
	if (!mIsPicCopy)
	{
		releasePreviewFrame(pbuf->index);
	}

	DBG_TIME_END("Take smart picture", 0);
	

	return true;
}

void V4L2CameraDevice::startSmartPicture()
{
	F_LOG;

	mSmartPictureDone = false;

	DBG_TIME_AVG_INIT(TAG_SMART_PICTURE);
}

void V4L2CameraDevice::stopSmartPicture()
{
	F_LOG;

	if (mSmartPictureDone)
	{
		LOGD("Smart picture has already stopped");
		return;
	}
	
	mSmartPictureDone = true;

	DBG_TIME_AVG_END(TAG_SMART_PICTURE, "picture enc");
}


// continuous picture
bool V4L2CameraDevice::continuousPictureThread()
{
	V4L2BUF_t * pbuf = (V4L2BUF_t *)OSAL_Dequeue(&mQueueBufferPicture);
	if (pbuf == NULL)
	{
		LOGV("continuousPictureThread queue no buffer, sleep...");
		pthread_mutex_lock(&mContinuousPictureMutex);
		pthread_cond_wait(&mContinuousPictureCond, &mContinuousPictureMutex);
		pthread_mutex_unlock(&mContinuousPictureMutex);
		return true;
	}
	
	Mutex::Autolock locker(&mObjectLock);
	if (mMapMem.mem[pbuf->index] == NULL
		|| pbuf->addrPhyY == 0)
	{
		LOGV("picture buffer have been released...");
		return true;
	}
	
	DBG_TIME_AVG_AREA_IN(TAG_CONTINUOUS_PICTURE);

	// reach the max number of pictures
	if (mContinuousPictureCnt >= mContinuousPictureMax)
	{
		mTakePictureState = TAKE_PICTURE_NULL;
		stopContinuousPicture();
		releasePreviewFrame(pbuf->index);
		return true;
	}

	// apk stop continuous pictures
	if (!mContinuousPictureStarted)
	{
		mTakePictureState = TAKE_PICTURE_NULL;
		releasePreviewFrame(pbuf->index);
		return true;
	}

	bool ret = mCallbackNotifier->takePicture((void*)pbuf, true);
	if (ret)
	{
		mContinuousPictureCnt++;
	
		DBG_TIME_AVG_AREA_OUT(TAG_CONTINUOUS_PICTURE);
	}
	else
	{
		// LOGW("do not encoder jpeg");
	}
	
	releasePreviewFrame(pbuf->index);

	return true;
}

void V4L2CameraDevice::startContinuousPicture()
{
	F_LOG;

	mContinuousPictureCnt = 0;
	mContinuousPictureStarted = true;
	mContinuousPictureStartTime = systemTime(SYSTEM_TIME_MONOTONIC);

	DBG_TIME_AVG_INIT(TAG_CONTINUOUS_PICTURE);
}

void V4L2CameraDevice::stopContinuousPicture()
{
	F_LOG;

	if (!mContinuousPictureStarted)
	{
		LOGD("Continuous picture has already stopped");
		return;
	}
	
	mContinuousPictureStarted = false;

	nsecs_t time = (systemTime(SYSTEM_TIME_MONOTONIC) - mContinuousPictureStartTime)/1000000;
	LOGD("Continuous picture cnt: %d, use time %lld(ms)", mContinuousPictureCnt, time);
	if (time != 0)
	{
		LOGD("Continuous picture %f(fps)", (float)mContinuousPictureCnt/(float)time * 1000);
	}

	DBG_TIME_AVG_END(TAG_CONTINUOUS_PICTURE, "picture enc");
}

void V4L2CameraDevice::setContinuousPictureCnt(int cnt)
{
	F_LOG;
	mContinuousPictureMax = cnt;
}

bool V4L2CameraDevice::isContinuousPictureTime()
{
	if (mTakePictureState == TAKE_PICTURE_CONTINUOUS_FAST)
	{
		return true;
	}
	
    timeval cur_time;
    gettimeofday(&cur_time, NULL);
    const uint64_t cur_mks = cur_time.tv_sec * 1000000LL + cur_time.tv_usec;
    if ((cur_mks - mContinuousPictureLast) >= mContinuousPictureAfter) {
        mContinuousPictureLast = cur_mks;
        return true;
    }
    return false;
}

bool V4L2CameraDevice::isPreviewTime()
{
	if (mVideoHint != true)
	{
		return true;
	}
	
    timeval cur_time;
    gettimeofday(&cur_time, NULL);
    const uint64_t cur_mks = cur_time.tv_sec * 1000000LL + cur_time.tv_usec;
    if ((cur_mks - mPreviewLast) >= mPreviewAfter) {
        mPreviewLast = cur_mks;
        return true;
    }
    return false;
}

void V4L2CameraDevice::waitFaceDectectTime()
{
    timeval cur_time;
    gettimeofday(&cur_time, NULL);
    const uint64_t cur_mks = cur_time.tv_sec * 1000000LL + cur_time.tv_usec;
	
    if ((cur_mks - mFaceDectectLast) >= mFaceDectectAfter)
	{
		mFaceDectectLast = cur_mks;
	}	
	else
	{
		usleep(mFaceDectectAfter - (cur_mks - mFaceDectectLast));
	    gettimeofday(&cur_time, NULL);
		mFaceDectectLast = cur_time.tv_sec * 1000000LL + cur_time.tv_usec;
	}
}

int V4L2CameraDevice::getCurrentFaceFrame(void * frame)
{
	if((mCameraType == CAMERA_TYPE_TVIN_NTSC) || (mCameraType == CAMERA_TYPE_TVIN_PAL))
		return -1;

    int len = 0;
	if (frame == NULL)
	{
		LOGE("getCurrentFrame: error in null pointer");
		return -1;
	}

	pthread_mutex_lock(&mCaptureMutex);
	// stop capture
	if (mCaptureThreadState != CAPTURE_STATE_STARTED)
	{
		LOGW("capture thread dose not started");
		pthread_mutex_unlock(&mCaptureMutex);
		return -1;
	}
	pthread_mutex_unlock(&mCaptureMutex);
	
	//waitFaceDectectTime();

    Mutex::Autolock locker(&mObjectLock);
	
	if (mCurrentV4l2buf == NULL
		|| mCurrentV4l2buf->addrVirY == 0)
	{
		LOGW("frame buffer not ready");
		return -1;
	}

	if ((mCurrentV4l2buf->isThumbAvailable == 1)
		&& (mCurrentV4l2buf->thumbUsedForPreview == 1))
	{
		memcpy(frame, 
				(void*)mCurrentV4l2buf->addrVirY + ALIGN_4K(ALIGN_16B(mCurrentV4l2buf->width) * mCurrentV4l2buf->height * 3 / 2), 
				ALIGN_16B(mCurrentV4l2buf->thumbWidth) * mCurrentV4l2buf->thumbHeight);

		len = ALIGN_16B(mCurrentV4l2buf->thumbWidth) * mCurrentV4l2buf->thumbHeight;
	}
	else
	{
		//LOGW("frame buffer copy");
		memcpy(frame, (void*)mCurrentV4l2buf->addrVirY, mCurrentV4l2buf->width * mCurrentV4l2buf->height);
		len =  mCurrentV4l2buf->width * mCurrentV4l2buf->height;
	}

	//return 0;
	return len;
}

// -----------------------------------------------------------------------------
// extended interfaces here <***** star *****>
// -----------------------------------------------------------------------------
int V4L2CameraDevice::openCameraDev(HALCameraInfo * halInfo)
{
	F_LOG;

	int ret = -1;
	struct v4l2_input inp;
	struct v4l2_capability cap; 
	char dev_node[16];
	bool foundDev = false;
	
	if (halInfo == NULL)
	{
		LOGE("error HAL camera info");
		return -1;
	}
#ifdef SUPPORT_VIRTUAL_CAMERA
    if (halInfo->device_id == 2) {
        if (mCsiRunning) {
            mCameraType = CAMERA_TYPE_VIR;
            return NO_ERROR;
        }
    }
#endif
	// open V4L2 device
    //-----------------------------------------------
    //If "video0" not exist, use others instead
    //Modified by Microphone
    //2013-11-14
    //-----------------------------------------------
#if 1
       if (!strcmp(halInfo->device_name, "/dev/video0"))
        {
            for (int i = 0; i < CVBS_DEVICE_NUM_START; i++)
           {
                sprintf(dev_node, "/dev/video%d", i);
                ret = access(dev_node, F_OK);
                if(ret == 0)
                {
                    LOGV("open preview camera=%s ok!!!",dev_node);
                    foundDev = true;
                    break;
                }
           }
           if(!foundDev)
           {
               ALOGW("did not find preview video0,continue to find from video8");
			   usleep(100000);
               for (int i = CVBS_DEVICE_NUM_START+4; i < 0xff; i++)
               {
                        sprintf(dev_node, "/dev/video%d", i);
                        ret = access(dev_node, F_OK);
                        if(ret == 0)
                        {
                            ALOGV("open preview ok !replace video0 with %s",dev_node);
                            foundDev = true;
                            break;
                        }
               }
           }
        }
        else if(!strcmp(halInfo->device_name, "/dev/video1"))
        {
            sprintf(dev_node, "/dev/video1");
            ret = access(dev_node, F_OK);
            if(ret == 0)
            {
				LOGV("open preview camera=/dev/video1 ok!!!");
				foundDev = true;
            }

           if(!foundDev)
           {
               ALOGW("did not find /dev/video1");
           }
        }
        else if((access(halInfo->device_name, F_OK)) == 0)
       {
                  strcpy(dev_node,halInfo->device_name);
                  foundDev = true;
                  LOGV("open cvbs camera id=%s ok!!!",dev_node);
       }
       if(!foundDev)
       {
            LOGE("did not find  halInfo->device_name=%s", halInfo->device_name);
             return -1;
       }


#else
    if((access(halInfo->device_name, F_OK)) == 0)
    {
        strcpy(dev_node,halInfo->device_name);
    }
    else
    {
        for (int i = 0; i < MAX_NUM_OF_CAMERAS; i++)
        {
            sprintf(dev_node, "/dev/video%d", i);
            ret = access(dev_node, F_OK);
            if(ret == 0)
            {
                break;
            }
        }
    }
#endif
    mCameraFd = open(dev_node, O_RDWR | O_NONBLOCK, 0);
    if (mCameraFd == -1)
    {
        LOGE("ERROR opening %s: %s", dev_node, strerror(errno));
        return -1;
    }
    //-------------------------------------------------
	// check v4l2 device capabilities
	ret = ioctl (mCameraFd, VIDIOC_QUERYCAP, &cap); 
    if (ret < 0) 
	{ 
        LOGE("Error opening device: unable to query device."); 
        goto END_ERROR;
    } 

    if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) 
	{ 
        LOGE("Error opening device: video capture not supported."); 
        goto END_ERROR;
    } 
  
    if ((cap.capabilities & V4L2_CAP_STREAMING) == 0) 
	{ 
        LOGE("Capture device does not support streaming i/o"); 
        goto END_ERROR;
    } 

	if (!strcmp((char *)cap.driver, "uvcvideo"))
	{
		mCameraType = CAMERA_TYPE_UVC;
        LOGD("mCameraType = CAMERA_TYPE_UVC");
	}
	else if (!strcmp((char *)cap.driver, "sunxi-tvd" ))
	{
		int tvinType;
		tvinType = getTVINSystemType(mCameraFd);
		if(tvinType == TVD_PAL )
		{
			mCameraType = CAMERA_TYPE_TVIN_PAL;
			LOGD("mCameraType = CAMERA_TYPE_TVIN_PAL");
		}
		else if(tvinType == TVD_NTSC )
		{
			mCameraType = CAMERA_TYPE_TVIN_NTSC;
			LOGD("mCameraType = CAMERA_TYPE_TVIN_NTSC");
		}
		else if(tvinType == TVD_YPBPR )
		{
			mCameraType = CAMERA_TYPE_TVIN_YPBPR;
			LOGD("mCameraType = CAMERA_TYPE_TVIN_YPBPR");
		}
		else
		{
		#ifdef DEFAULT_CVBS_CAMERA_TYPE_PAL
			mCameraType = CAMERA_TYPE_TVIN_PAL;
			LOGD("mCameraType = CAMERA_TYPE_TVIN_PAL");
		#else	
			mCameraType = CAMERA_TYPE_TVIN_NTSC;
			LOGD("Default mCameraType = CAMERA_TYPE_TVIN_NTSC;");
		#endif
		}
		
	}
	else
	{
		mCameraType = CAMERA_TYPE_CSI;
        LOGD("mCameraType = CAMERA_TYPE_CSI");
	}
	LOGD("The name of the Camera is '%s'",cap.card);

	if (mCameraType == CAMERA_TYPE_CSI)
	{
		// uvc do not need to set input
		inp.index = 0;
		if (-1 == ioctl (mCameraFd, VIDIOC_S_INPUT, &inp))
		{
			LOGE("VIDIOC_S_INPUT error id=%d!",halInfo->device_id);
			goto END_ERROR;
		}
        
       halInfo->facing = CAMERA_FACING_FRONT;
        mIsThumbUsedForVideo = true;
    }

	if (mCameraType == CAMERA_TYPE_UVC)
	{
		// try to support this format: NV21, YUYV
		// we do not support mjpeg camera now
#if 0
		mCameraList = new CameraList();
		bool isSupported = false;
		char *res = strstr(mCameraList->mCameraDeviceList,(char const *)cap.card);
		//if(mCameraList->mCameraDeviceList != NULL)LOGI("MJPEG Camera '%s',mCameraList->mCameraDeviceList lenght = %d",mCameraList->mCameraDeviceList,strlen((char const *)mCameraList->mCameraDeviceList));
		//if(res != NULL)LOGI("MJPEG Camera '%s',sizeof = %d",res,strlen((char const *)res));
		//LOGI("MJPEG Camera '%s',sizeof = %d",cap.card,strlen((char const *)cap.card));
		if(res != NULL){
		    if(!strncmp((char const *)cap.card,(char const *)res,strlen((char const *)cap.card))){
			    isSupported = true;
			    LOGI("MJPEG Camera '%s' is Supported",cap.card);
		    }
		}else{
			LOGI("MJPEG Camera '%s' is not Supported",cap.card);
		}
		delete mCameraList;
#endif		
		
        if (tryFmt(V4L2_PIX_FMT_NV21) == OK)
        {
              mCaptureFormat = V4L2_PIX_FMT_NV21;
              LOGE("capture format: V4L2_PIX_FMT_NV21");
        }
       
  		else if(tryFmt(V4L2_PIX_FMT_H264) == OK)
		{
			mCaptureFormat = V4L2_PIX_FMT_H264;
			LOGV("capture format: V4L2_PIX_FMT_H264");
		}
		else if(tryFmt(V4L2_PIX_FMT_YUYV) == OK)
		{
			mCaptureFormat = V4L2_PIX_FMT_YUYV;		// maybe usb camera
			LOGV("capture format: V4L2_PIX_FMT_YUYV");
		}
		 else  if(tryFmt(V4L2_PIX_FMT_MJPEG) == OK)
		{
			mCaptureFormat = V4L2_PIX_FMT_MJPEG;		// maybe usb camera
			LOGE("capture format: V4L2_PIX_FMT_MJPEG");
		}
		else
		{
			LOGE("driver should surpport NV21/NV12 or YUYV format, but it not!");
			goto END_ERROR;
		}
	}

	return OK;

END_ERROR:

	if (mCameraFd != NULL)
	{
		close(mCameraFd);
		mCameraFd = NULL;
	}
	
	return -1;
}

void V4L2CameraDevice::closeCameraDev()
{
	F_LOG;
	
	if (mCameraFd != NULL)
	{
		close(mCameraFd);
		mCameraFd = NULL;
	}
}

int V4L2CameraDevice::v4l2SetVideoParams(int width, int height, uint32_t pix_fmt)
{
	int ret = UNKNOWN_ERROR;
	struct v4l2_format format;
	int i = 0;

	LOGV("%s, line: %d, w: %d, h: %d, pfmt: %d", 
		__FUNCTION__, __LINE__, width, height, pix_fmt);
	
	memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
    format.fmt.pix.width  = width;
    format.fmt.pix.height = height;
	if(mCaptureFormat == V4L2_PIX_FMT_MJPEG)
	{
		format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
		LOGV("set capture format: V4L2_PIX_FMT_MJPEG");
	}
	else if (mCaptureFormat == V4L2_PIX_FMT_YUYV)
	{
    	format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; 
		LOGV("set capture format: V4L2_PIX_FMT_YUYV");
	}
	else if (mCaptureFormat == V4L2_PIX_FMT_H264)
	{
		format.fmt.pix.pixelformat = V4L2_PIX_FMT_H264;
		LOGV("set capture format: V4L2_PIX_FMT_H264");
	}
	else
	{
		format.fmt.pix.pixelformat = pix_fmt; 
	}
	format.fmt.pix.field = V4L2_FIELD_NONE;

	struct v4l2_pix_format sub_fmt;
	if (mHalCameraInfo.fast_picture_mode)
	{
		memset(&sub_fmt, 0, sizeof(sub_fmt));
		
		int scale = getSuitableThumbScale(width, height);
		if (scale <= 0)
		{
			LOGE("error thumb scale: %d, src_size: %dx%d", scale, width, height);
			return UNKNOWN_ERROR;
		}
		
		format.fmt.pix.subchannel = &sub_fmt;
		format.fmt.pix.subchannel->width = format.fmt.pix.width / scale;
		format.fmt.pix.subchannel->height = format.fmt.pix.height / scale;
		format.fmt.pix.subchannel->pixelformat = pix_fmt;
		format.fmt.pix.subchannel->field = V4L2_FIELD_NONE;	
		LOGV("to camera params: w: %d, h: %d, sub: %dx%d, pfmt: %d, pfield: %d", 
			format.fmt.pix.width, format.fmt.pix.height,
			format.fmt.pix.subchannel->width, format.fmt.pix.subchannel->height, pix_fmt, V4L2_FIELD_NONE);
	}

	if(mCameraType == CAMERA_TYPE_TVIN_NTSC)
	{
		format.fmt.pix.width  = 720;
		format.fmt.pix.height = 480;
		format.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;
		format.fmt.pix.pixelformat = V4L2_PIX_FMT_NV21;	
//		format.fmt.pix.priv = 0;/*cvbs/ypbprp/ypbpri*/	

	}
	if(mCameraType == CAMERA_TYPE_TVIN_PAL)
	{
		format.fmt.pix.width  = 720;
		format.fmt.pix.height = 576;
		format.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;
		format.fmt.pix.pixelformat = V4L2_PIX_FMT_NV21;	
//		format.fmt.pix.priv = 0;/*cvbs/ypbprp/ypbpri*/	
	}

	if(mCameraType == CAMERA_TYPE_TVIN_YPBPR)
	{
		memset(&format, 0, sizeof(struct v4l2_format));	
		format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		i = 0;
		while (ioctl(mCameraFd, VIDIOC_G_FMT, &format) &&(i++ < 20)) 
		{		
			LOGV("No signal detected\n");		
			return -1;
		}	
		
		format.type				 = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		format.fmt.pix.pixelformat = V4L2_PIX_FMT_NV21;	
		mFrameWidth = format.fmt.pix.width ;
		mFrameHeight = format.fmt.pix.height;
	}

	ret = ioctl(mCameraFd, VIDIOC_S_FMT, &format); 
	if (ret < 0) 
	{ 
		LOGE("VIDIOC_S_FMT Failed: %s", strerror(errno)); 
		return ret; 
	} 

	mFrameWidth = format.fmt.pix.width;
	mFrameHeight = format.fmt.pix.height;

	if (mHalCameraInfo.fast_picture_mode)
	{
		mThumbWidth = format.fmt.pix.subchannel->width;
		mThumbHeight = format.fmt.pix.subchannel->height;
	}
	if(mCameraType == CAMERA_TYPE_TVIN_NTSC){
		mFrameWidth = 720;
		mFrameHeight = 480;
	}
	if(mCameraType == CAMERA_TYPE_TVIN_PAL){
		mFrameWidth = 720;
		mFrameHeight = 576;
	}
	LOGV("camera params: w: %d, h: %d, sub: %dx%d, pfmt: %d, pfield: %d", 
		mFrameWidth, mFrameHeight, mThumbWidth, mThumbHeight, pix_fmt, V4L2_FIELD_NONE);

	return OK;
}

int V4L2CameraDevice::v4l2ReqBufs(int * buf_cnt)
{
	F_LOG;
	int ret = UNKNOWN_ERROR;
	struct v4l2_requestbuffers rb;

	LOGV("TO VIDIOC_REQBUFS count: %d", *buf_cnt);
	
	memset(&rb, 0, sizeof(rb));
    rb.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
    rb.memory = V4L2_MEMORY_MMAP; 
    rb.count  = *buf_cnt; 
	
	ret = ioctl(mCameraFd, VIDIOC_REQBUFS, &rb); 
    if (ret < 0) 
	{ 
        LOGE("Init: VIDIOC_REQBUFS failed: %s", strerror(errno)); 
		return ret;
    } 

	*buf_cnt = rb.count;
	LOGV("VIDIOC_REQBUFS count: %d", *buf_cnt);

	return OK;
}

int V4L2CameraDevice::v4l2QueryBuf()
{
	F_LOG;
	int ret = UNKNOWN_ERROR;
	struct v4l2_buffer buf;
	
	for (int i = 0; i < mBufferCnt; i++) 
	{  
        memset (&buf, 0, sizeof (struct v4l2_buffer)); 
		buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
		buf.memory = V4L2_MEMORY_MMAP; 
		buf.index  = i; 
		
		ret = ioctl (mCameraFd, VIDIOC_QUERYBUF, &buf); 
        if (ret < 0) 
		{ 
            LOGE("Unable to query buffer (%s)", strerror(errno)); 
            return ret; 
        } 

 
        mMapMem.mem[i] = mmap (0, buf.length, 
                            PROT_READ | PROT_WRITE, 
                            MAP_SHARED, 
                            mCameraFd, 
                            buf.m.offset); 
		mMapMem.length = buf.length;
		LOGV("index: %d, mem: %x, len: %x, offset: %x", i, (int)mMapMem.mem[i], buf.length, buf.m.offset);
 
        if (mMapMem.mem[i] == MAP_FAILED) 
		{ 
			LOGE("Unable to map buffer (%s)", strerror(errno)); 
            return -1; 
        } 

		// start with all buffers in queue
        ret = ioctl(mCameraFd, VIDIOC_QBUF, &buf); 
        if (ret < 0) 
		{ 
            LOGE("VIDIOC_QBUF Failed"); 
            return ret; 
        } 

		if (mCameraType == CAMERA_TYPE_UVC && mCaptureFormat == V4L2_PIX_FMT_H264)		// star to do
		{
			int buffer_len =ALIGN_32B(mFrameWidth) * ALIGN_32B(mFrameHeight) * 3 / 2;
		//int buffer_len = mFrameWidth * mFrameHeight *2;
		    #ifdef USE_ION_MEM_ALLOCATOR
			mVideoBuffer.buf_vir_addr[i] = (int)ion_alloc_alloc(buffer_len);
			mVideoBuffer.buf_phy_addr[i] = ion_alloc_vir2phy((void*)mVideoBuffer.buf_vir_addr[i]);
			#elif USE_CDX_NEW_MEM_API
			mVideoBuffer.buf_vir_addr[i] = (int)CdcMemPalloc(mCameraMemops,buffer_len);
			mVideoBuffer.buf_phy_addr[i] = (int)CdcMemGetPhysicAddressCpu(mCameraMemops,(void*)mVideoBuffer.buf_vir_addr[i]);	

	        #else USE_SUNXI_MEM_ALLOCATOR
			mVideoBuffer.buf_vir_addr[i] = (int)sunxi_alloc_alloc(buffer_len);
			mVideoBuffer.buf_phy_addr[i] = sunxi_alloc_vir2phy((void*)mVideoBuffer.buf_vir_addr[i]);
		    #endif
			LOGV("video buffer: index: %d, vir: %x, phy: %x, len: %x", 
					i, mVideoBuffer.buf_vir_addr[i], mVideoBuffer.buf_phy_addr[i], buffer_len);
			
			memset((void*)mVideoBuffer.buf_vir_addr[i], 0x10, mFrameWidth * mFrameHeight);
			memset((void*)mVideoBuffer.buf_vir_addr[i] + mFrameWidth * mFrameHeight, 
					0x80, mFrameWidth * mFrameHeight / 2);
		}
	} 

	return OK;
}

int V4L2CameraDevice::v4l2StartStreaming()
{
	F_LOG;
	int ret = UNKNOWN_ERROR; 
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	
  	ret = ioctl (mCameraFd, VIDIOC_STREAMON, &type); 
	if (ret < 0) 
	{ 
		LOGE("StartStreaming: Unable to start capture: %s", strerror(errno)); 
		return ret; 
	} 

	return OK;
}

int V4L2CameraDevice::v4l2StopStreaming()
{
	F_LOG;
	int ret = UNKNOWN_ERROR; 
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	
	ret = ioctl (mCameraFd, VIDIOC_STREAMOFF, &type); 
	if (ret < 0) 
	{ 
		LOGE("StopStreaming: Unable to stop capture: %s", strerror(errno)); 
		return ret; 
	} 
	LOGV("V4L2Camera::v4l2StopStreaming OK");

	return OK;
}

int V4L2CameraDevice::v4l2UnmapBuf()
{
	F_LOG;
	int ret = UNKNOWN_ERROR;
	
	for (int i = 0; i < mBufferCnt; i++) 
	{
		ret = munmap(mMapMem.mem[i], mMapMem.length);
        if (ret < 0) 
		{
            LOGE("v4l2CloseBuf Unmap failed"); 
			return ret;
		}

		mMapMem.mem[i] = NULL;

		if (mVideoBuffer.buf_vir_addr[i] != 0)
		{
		    #ifdef USE_ION_MEM_ALLOCATOR
			ion_alloc_free((void*)mVideoBuffer.buf_vir_addr[i]);
			mVideoBuffer.buf_phy_addr[i] = 0;
			#elif USE_CDX_NEW_MEM_API
			CdcMemPfree(mCameraMemops,(void*)mVideoBuffer.buf_vir_addr[i]);
			mVideoBuffer.buf_phy_addr[i] = 0;
		    #else USE_SUNXI_MEM_ALLOCATOR
			sunxi_alloc_free((void*)mVideoBuffer.buf_vir_addr[i]);
			mVideoBuffer.buf_phy_addr[i] = 0;
		    #endif
		}
	}
	mVideoBuffer.buf_unused = NB_BUFFER;
	mVideoBuffer.read_id = 0;
	mVideoBuffer.read_id = 0;
	
	return OK;
}

void V4L2CameraDevice::releasePreviewFrame(int index)
{
	int ret = UNKNOWN_ERROR;
	struct v4l2_buffer buf;
	
	//if((mCameraManager != NULL) && (mCameraManager->isOviewEnable())) 
	//	return;
	
	pthread_mutex_lock(&mCaptureMutex);
	//LOGD("releasePreviewFrame ID: %d cid=%d cur ava =%d now cnt =%d", index,mHalCameraInfo.device_id,mCurAvailBufferCnt,mV4l2buf[index].refCnt);

	// decrease buffer reference count first, if the reference count is no more than 0, release it.
	if (mV4l2buf[index].refCnt > 0
		&& --mV4l2buf[index].refCnt == 0)
	{
		memset(&buf, 0, sizeof(v4l2_buffer));
		buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	    buf.memory = V4L2_MEMORY_MMAP; 
		buf.index = index;
		
		
	    ret = ioctl(mCameraFd, VIDIOC_QBUF, &buf); 
	    if (ret != 0) 
		{
	        LOGE("releasePreviewFrame: VIDIOC_QBUF Failed: index = %d, ret = %d, %s", 
				buf.index, ret, strerror(errno)); 
	    }
		else
		{
			memset(&mV4l2buf[index],0,sizeof(V4L2BUF_t));
			mCurAvailBufferCnt++;
		}
		//LOGD("r ID: %d cid=%d cur ava bug =%d", buf.index,mHalCameraInfo.device_id,mCurAvailBufferCnt);
	}
#if 0
	int i;
	long long		timeStamp=systemTime();
	
	for(i=0;i<NB_BUFFER;i++){
		if((mV4l2buf[i].timeStamp!=0)&&(timeStamp - mV4l2buf[i].timeStamp>5000000000LL))//if a buf 500ms have not release force release it
		{
			memset(&buf, 0, sizeof(v4l2_buffer));
			buf.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
			buf.memory = V4L2_MEMORY_MMAP; 
			buf.index = i;
			
			ret = ioctl(mCameraFd, VIDIOC_QBUF, &buf); 
			if (ret != 0) 
			{
				LOGE("x releasePreviewFrame: VIDIOC_QBUF Failed: index = %d, ret = %d, %s", 
					buf.index, ret, strerror(errno)); 
			}
			else
			{
				memset(&mV4l2buf[i],0,sizeof(V4L2BUF_t));
				mCurAvailBufferCnt++;
			}
			ALOGW("find buf id %d time[ %lld] not release for 500ms,now[%lld]",i, mV4l2buf[i].timeStamp,timeStamp);

		}
	}
#endif
	pthread_mutex_unlock(&mCaptureMutex);
}

int V4L2CameraDevice::getPreviewFrame(v4l2_buffer *buf)
{
	int ret = UNKNOWN_ERROR;
	
	buf->type   = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
    buf->memory = V4L2_MEMORY_MMAP; 
 
    ret = ioctl(mCameraFd, VIDIOC_DQBUF, buf); 
    if (ret < 0) 
	{ 
        LOGW("GetPreviewFrame: VIDIOC_DQBUF Failed, %s", strerror(errno)); 
        return __LINE__; 			// can not return false
    }

    testFrameRate();
    return OK;
}

int V4L2CameraDevice::tryFmt(int format)
{	
	struct v4l2_fmtdesc fmtdesc;
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	for(int i = 0; i < 12; i++)
	{
		fmtdesc.index = i;
		if (-1 == ioctl (mCameraFd, VIDIOC_ENUM_FMT, &fmtdesc))
		{
			break;
		}
		LOGV("format index = %d, name = %s, v4l2 pixel format = %x\n",
			i, fmtdesc.description, fmtdesc.pixelformat);

		if (fmtdesc.pixelformat == format)
		{
			return OK;
		}
	}

	return -1;
}

int V4L2CameraDevice::tryFmtSize(int * width, int * height)
{
	F_LOG;
	int ret = -1;
	struct v4l2_format fmt;

	LOGV("V4L2Camera::TryFmtSize: w: %d, h: %d", *width, *height);

#ifdef SUPPORT_VIRTUAL_CAMERA
    if(mCameraType == CAMERA_TYPE_VIR && mCsiRunning) {
        return NO_ERROR;
    }
#endif

	memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; 
    fmt.fmt.pix.width  = *width; 
    fmt.fmt.pix.height = *height; 
	if(mCaptureFormat == V4L2_PIX_FMT_MJPEG)
	{
		fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	}
	else if (mCaptureFormat == V4L2_PIX_FMT_YUYV)
	{
		fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
	}
	else if (mCaptureFormat == V4L2_PIX_FMT_H264)
	{
		fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_H264;
	}
	else
	{
    	fmt.fmt.pix.pixelformat = mVideoFormat; 
	}
	fmt.fmt.pix.field = V4L2_FIELD_NONE;

	ret = ioctl(mCameraFd, VIDIOC_TRY_FMT, &fmt); 
	if (ret < 0) 
	{ 
		LOGE("VIDIOC_TRY_FMT Failed: %s", strerror(errno)); 
		return ret; 
	} 

	// driver surpport this size
	*width = fmt.fmt.pix.width; 
    *height = fmt.fmt.pix.height; 

	return 0;
}

int V4L2CameraDevice::setFrameRate(int rate)
{
	mFrameRate = rate;
	return OK;
}

int V4L2CameraDevice::getFrameRate()
{
	F_LOG;
	int ret = -1;

	struct v4l2_streamparm parms;
	parms.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	ret = ioctl (mCameraFd, VIDIOC_G_PARM, &parms);
	if (ret < 0) 
	{
		LOGE("VIDIOC_G_PARM getFrameRate error, %s", strerror(errno));
		return ret;
	}

	int numerator = parms.parm.capture.timeperframe.numerator;
	int denominator = parms.parm.capture.timeperframe.denominator;
	
	LOGV("frame rate: numerator = %d, denominator = %d", numerator, denominator);

	if (numerator != 0
		&& denominator != 0)
	{
		return denominator / numerator;
	}
	else
	{
		LOGW("unsupported frame rate: %d/%d", denominator, numerator);
		return 30;
	}
}

int V4L2CameraDevice::setImageEffect(int effect)
{
	F_LOG;
	int ret = -1;
	struct v4l2_control ctrl;

	ctrl.id = V4L2_CID_COLORFX;
	ctrl.value = effect;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		LOGV("setImageEffect failed!");
	else 
		LOGV("setImageEffect ok");

	return ret;
}

int V4L2CameraDevice::setWhiteBalance(int wb)
{
	struct v4l2_control ctrl;
	int ret = -1;

	ctrl.id = V4L2_CID_AUTO_N_PRESET_WHITE_BALANCE;
	ctrl.value = wb;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		LOGV("setWhiteBalance failed, %s", strerror(errno));
	else 
		LOGV("setWhiteBalance ok");

	return ret;
}

int V4L2CameraDevice::setTakePictureCtrl()
{
	struct v4l2_control ctrl;
	int ret = -1;
	if (mHalCameraInfo.fast_picture_mode){
		ctrl.id = V4L2_CID_TAKE_PICTURE;
		ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
		if (ret < 0)
			LOGV("setTakePictureCtrl failed, %s", strerror(errno));
		else 
			LOGV("setTakePictureCtrl ok");

		return ret;
	}
	return 0;
}

// ae mode
int V4L2CameraDevice::setExposureMode(int mode)
{
	F_LOG;
	int ret = -1;
	struct v4l2_control ctrl;

	ctrl.id = V4L2_CID_EXPOSURE_AUTO;
	ctrl.value = mode;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		LOGV("setExposureMode failed, %s", strerror(errno));
	else 
		LOGV("setExposureMode ok");

	return ret;
}

// ae compensation
int V4L2CameraDevice::setExposureCompensation(int val)
{
	F_LOG;
	int ret = -1;
	struct v4l2_control ctrl;

	ctrl.id = V4L2_CID_AUTO_EXPOSURE_BIAS;
	ctrl.value = val;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		LOGV("setExposureCompensation failed, %s", strerror(errno));
	else 
		LOGV("setExposureCompensation ok");

	return ret;
}

// ae compensation
int V4L2CameraDevice::setExposureWind(int num, void *wind)
{
	F_LOG;
	int ret = -1;
	struct v4l2_control ctrl;

	ctrl.id = V4L2_CID_AUTO_EXPOSURE_WIN_NUM;
	ctrl.value = num;
	//ctrl.user_pt = (unsigned int)wind;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		LOGE("setExposureWind failed, %s", strerror(errno));
	else 
		LOGV("setExposureWind ok");

	return ret;
}

// flash mode
int V4L2CameraDevice::setFlashMode(int mode)
{
	F_LOG;
	int ret = -1;
	struct v4l2_control ctrl;

	ctrl.id = V4L2_CID_FLASH_LED_MODE;
	ctrl.value = mode;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		LOGV("setFlashMode failed, %s", strerror(errno));
	else 
		LOGV("setFlashMode ok");

	return ret;
}

// af init
int V4L2CameraDevice::setAutoFocusInit()
{
	F_LOG;
	int ret = -1;
	struct v4l2_control ctrl;

	ctrl.id = V4L2_CID_AUTO_FOCUS_INIT;
	ctrl.value = 0;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		LOGE("setAutoFocusInit failed, %s", strerror(errno));
	else 
		LOGV("setAutoFocusInit ok");

	return ret;
}

// af release
int V4L2CameraDevice::setAutoFocusRelease()
{
	F_LOG;
	#ifdef SUPPORT_VIRTUAL_CAMERA
    if (mCameraType != CAMERA_TYPE_CSI  || (mCameraType == CAMERA_TYPE_VIR && mCsiRunning)){
        return 0;
    }
	#endif
	int ret = -1;
	struct v4l2_control ctrl;

	ctrl.id = V4L2_CID_AUTO_FOCUS_RELEASE;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		LOGE("setAutoFocusRelease failed, %s", strerror(errno));
	else 
		LOGV("setAutoFocusRelease ok");

	return ret;
}

// af range
int V4L2CameraDevice::setAutoFocusRange(int af_range)
{
	F_LOG;
	int ret = -1;
	struct v4l2_control ctrl;

	ctrl.id = V4L2_CID_FOCUS_AUTO;
	ctrl.value = 1;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		LOGV("setAutoFocusRange id V4L2_CID_FOCUS_AUTO failed, %s", strerror(errno));
	else 
		LOGV("setAutoFocusRange id V4L2_CID_FOCUS_AUTO ok");

	ctrl.id = V4L2_CID_AUTO_FOCUS_RANGE;
	ctrl.value = af_range;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		LOGV("setAutoFocusRange id V4L2_CID_AUTO_FOCUS_RANGE failed, %s", strerror(errno));
	else 
		LOGV("setAutoFocusRange id V4L2_CID_AUTO_FOCUS_RANGE ok");

	return ret;
}

// af wind
int V4L2CameraDevice::setAutoFocusWind(int num, void *wind)
{
	F_LOG;
	int ret = -1;
	struct v4l2_control ctrl;

	ctrl.id = V4L2_CID_AUTO_FOCUS_WIN_NUM;
	ctrl.value = num;
	//ctrl.user_pt = (unsigned int)wind;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		LOGE("setAutoFocusCtrl failed, %s", strerror(errno));
	else 
		LOGV("setAutoFocusCtrl ok");

	return ret;
}

// af start
int V4L2CameraDevice::setAutoFocusStart()
{
	F_LOG;
	int ret = -1;
	struct v4l2_control ctrl;

	ctrl.id = V4L2_CID_AUTO_FOCUS_START;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		LOGE("setAutoFocusStart failed, %s", strerror(errno));
	else 
		LOGV("setAutoFocusStart ok");

	return ret;
}

// get horizontal visual angle
int V4L2CameraDevice::getHorVisualAngle()
{
    F_LOG;
    int ret = -1;
    struct v4l2_control ctrl;

    if (mCameraType == CAMERA_TYPE_CSI){
        ctrl.id = V4L2_CID_HOR_VISUAL_ANGLE;
        ret = ioctl(mCameraFd, VIDIOC_G_CTRL, &ctrl);
        if (ret < 0) {
            LOGE("getHorVisualAngle failed(%s)", strerror(errno));
            return ret;
        }
        return ctrl.value;
    }

    return ret;
}

// get vertical visual angle
int V4L2CameraDevice::getVerVisualAngle()
{
    F_LOG;
    int ret = -1;
    struct v4l2_control ctrl;

    if (mCameraType == CAMERA_TYPE_CSI){
        ctrl.id = V4L2_CID_VER_VISUAL_ANGLE;
        ret = ioctl(mCameraFd, VIDIOC_G_CTRL, &ctrl);
        if (ret < 0) {
            LOGE("getVerVisualAngle failed(%s)", strerror(errno));
            return ret;
        }
        return ctrl.value;
    }

    return ret;
}
// af stop
int V4L2CameraDevice::setAutoFocusStop()
{
	F_LOG;
	#ifdef SUPPORT_VIRTUAL_CAMERA
    if (mCameraType != CAMERA_TYPE_CSI  || (mCameraType == CAMERA_TYPE_VIR && mCsiRunning)){
        return 0;
    }
	#endif
	int ret = -1;
	struct v4l2_control ctrl;

	ctrl.id = V4L2_CID_AUTO_FOCUS_STOP;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret < 0)
		LOGE("setAutoFocusStart failed, %s", strerror(errno));
	else 
		LOGV("setAutoFocusStart ok");

	return ret;
}

// get af statue
int V4L2CameraDevice::getAutoFocusStatus()
{
	//F_LOG;
	#ifdef SUPPORT_VIRTUAL_CAMERA
    if (mCameraType != CAMERA_TYPE_CSI  || (mCameraType == CAMERA_TYPE_VIR && mCsiRunning)){
        return 0;
    }
	#endif
	int ret = -1;
	struct v4l2_control ctrl;

	if (mCameraFd == NULL)
	{
		return 0xFF000000;
	}

	ctrl.id = V4L2_CID_AUTO_FOCUS_STATUS;
	ret = ioctl(mCameraFd, VIDIOC_G_CTRL, &ctrl);
	if (ret >= 0)
	{
		//LOGV("getAutoFocusCtrl ok");
	}
	
	return ret;
}

int V4L2CameraDevice::getLightValue(int *lightvalue)
{
#if 0
    if (mCameraFd < 0) {
        return NO_INIT;
    }
    struct isp_exif_attribute exif_attr;
    int ret = ioctl(mCameraFd, VIDIOC_ISP_EXIF_REQ, &exif_attr);
    if (ret < 0) {
        LOGE("VIDIOC_ISP_EXIF_REQ failed(%s)", strerror(errno));
        return UNKNOWN_ERROR;
    }
    *lightvalue = exif_attr.reserved[2];
#endif
    return NO_ERROR;
}

int V4L2CameraDevice::getSnrValue()
{
	//F_LOG;
	int ret = -1;
	struct v4l2_control ctrl;
	struct v4l2_queryctrl qc_ctrl;

	if (mCameraFd == NULL)
	{
		return 0xFF000000;
	}

	ctrl.id = V4L2_CID_GAIN;
	qc_ctrl.id = V4L2_CID_GAIN;

	if (-1 == ioctl (mCameraFd, VIDIOC_QUERYCTRL, &qc_ctrl))
    {
	   return 0;
    }
	
	ret = ioctl(mCameraFd, VIDIOC_G_CTRL, &ctrl);
	return ret;
}


int V4L2CameraDevice::set3ALock(int lock)
{
	F_LOG;
	int ret = -1;
	struct v4l2_control ctrl;
	
	ctrl.id = V4L2_CID_3A_LOCK;
	ctrl.value = lock;
	ret = ioctl(mCameraFd, VIDIOC_S_CTRL, &ctrl);
	if (ret >= 0)
		LOGV("set3ALock ok");

	return ret;
}

int V4L2CameraDevice::v4l2setCaptureParams()
{
	F_LOG;
	int ret = -1;
	
	struct v4l2_streamparm params;
	params.parm.capture.timeperframe.numerator = 1;
	params.parm.capture.timeperframe.denominator = mFrameRate;
	params.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (mTakePictureState == TAKE_PICTURE_NORMAL)
	{
		params.parm.capture.capturemode = V4L2_MODE_IMAGE;
	}
	else
	{
		params.parm.capture.capturemode = V4L2_MODE_VIDEO;
	}

	LOGV("VIDIOC_S_PARM mFrameRate: %d, capture mode: %d", mFrameRate, params.parm.capture.capturemode);

	ret = ioctl(mCameraFd, VIDIOC_S_PARM, &params);
	if (ret < 0)
		LOGE("v4l2setCaptureParams failed, %s", strerror(errno));
	else 
		LOGV("v4l2setCaptureParams ok");

	return ret;
}

int V4L2CameraDevice::enumSize(char * pSize, int len)
{
	if (pSize == NULL)
	{
		LOGE("error input params");
		return -1;
	}

	char str[16];
	memset(str, 0, 16);
	memset(pSize, 0, len);

	if(mCameraType == CAMERA_TYPE_TVIN_NTSC)
	{
		sprintf(str, "%dx%d", 720, 480);
		strcat(pSize, str);
		return OK;
	}
    if(mCameraType == CAMERA_TYPE_TVIN_PAL)
	{
		sprintf(str, "%dx%d", 720, 576);
		strcat(pSize, str);
		return OK;
	}

	struct v4l2_frmsizeenum size_enum;
	size_enum.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	size_enum.pixel_format = mCaptureFormat;
	
	for(int i = 0; i < 20; i++)
	{
		size_enum.index = i;
		if (-1 == ioctl (mCameraFd, VIDIOC_ENUM_FRAMESIZES, &size_enum))
		{
			break;
		}
		LOGV("format index = %d, size_enum: %dx%d", i, size_enum.discrete.width, size_enum.discrete.height);
		sprintf(str, "%dx%d", size_enum.discrete.width, size_enum.discrete.height);
		if (i != 0)
		{
			strcat(pSize, ",");
		}
		strcat(pSize, str);
	}

	return OK;
}

int V4L2CameraDevice::getFullSize(int * full_w, int * full_h)
{
	if(mCameraType == CAMERA_TYPE_TVIN_NTSC)
	{
		*full_w = 720;
		*full_h = 480;
		LOGV("getFullSize: %dx%d", *full_w, *full_h);
		return OK;
	}
	if(mCameraType == CAMERA_TYPE_TVIN_PAL)
	{
		*full_w = 720;
		*full_h = 576;
		LOGV("getFullSize: %dx%d", *full_w, *full_h);
		return OK;
	}
	struct v4l2_frmsizeenum size_enum;
	size_enum.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	size_enum.pixel_format = mCaptureFormat;
	size_enum.index = 0;
	if (-1 == ioctl (mCameraFd, VIDIOC_ENUM_FRAMESIZES, &size_enum))
	{
		LOGE("getFullSize failed");
		return -1;
	}
	
	*full_w = size_enum.discrete.width;
	*full_h = size_enum.discrete.height;

	LOGV("getFullSize: %dx%d", *full_w, *full_h);
	
	return OK;
}

int V4L2CameraDevice::getSuitableThumbScale(int full_w, int full_h)
{
	int scale = 1;
	if(mIsThumbUsedForVideo == true)
	{
		scale = 2;
	}
	if (full_w*full_h > 10*1024*1024)		//maybe 12m,13m,16m
		return 2;
	else if(full_w*full_h > 4.5*1024*1024)	//maybe 5m,8m
		return 2;
	else return scale;						//others
#if 0
	if ((full_w == 4608)
		&& (full_h == 3456))
	{
		return 4;	// 1000x750
	}
	if ((full_w == 3840)
		&& (full_h == 2160))
	{
		return 4;	// 1000x750
	}
	if ((full_w == 4000)
		&& (full_h == 3000))
	{
		return 4;	// 1000x750
	}
	else if ((full_w == 3264)
		&& (full_h == 2448))
	{
		return 2;	// 1632x1224
	}
	else if ((full_w == 2592)
		&& (full_h == 1936))
	{
		return 2;	// 1296x968
	}
	else if ((full_w == 1280)
		&& (full_h == 960))
	{
		return 1 * scale;	// 1280x960
	}
	else if ((full_w == 1920)
		&& (full_h == 1080))
	{
		return 2;	// 960x540
	}
	else if ((full_w == 1280)
		&& (full_h == 720))
	{
		
		return 1 * scale;	// 1280x720
	}
	else if ((full_w == 640)
		&& (full_h == 480))
	{
		return 1;	// 640x480
	}

	LOGW("getSuitableThumbScale unknown size: %dx%d", full_w, full_h);
	return 1;		// failed
#endif
    return 1;       // failed
}

int V4L2CameraDevice::getTVINSystemType(int fd)
{
	struct v4l2_format format;
	struct v4l2_frmsizeenum frmsize;
	enum v4l2_buf_type type;
	int i = 0;	
	int temp_height = 0;

	
	memset(&format, 0, sizeof(struct v4l2_format));
	memset(&frmsize, 0, sizeof(struct v4l2_frmsizeenum));	
	frmsize.pixel_format = V4L2_PIX_FMT_NV21;	
	frmsize.index = 0;	     
	while ((!ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmsize))&&(i < 20)) 
	{	
		LOGV("i = %d\n", i);		
		LOGV("framesize width = %d, height = %d\n",frmsize.discrete.width, frmsize.discrete.height);	
		i++;
		frmsize.index++;	
	}

	memset(&format, 0, sizeof(struct v4l2_format));	
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	
#ifdef DEFAULT_CVBS_CAMERA_TYPE_PAL
	i = 0;
	while (ioctl(fd, VIDIOC_G_FMT, &format) &&(i++ < 20))  //this will use 200ms at lease
	{		
		printf("get tvin signal failed.\n");		
		return -1;
	}	
	//if you dont want to waste 200ms,try to set pix width and height directory(haven't test)
	//format.fmt.pix.width = 720;   
	//format.fmt.pix.height = 576;
#else
	format.fmt.pix.width = 720;
	format.fmt.pix.height = 480;
#endif

	LOGV("width= %d, height =[%d]\n",	format.fmt.pix.width, format.fmt.pix.height);	


	format.fmt.pix.pixelformat = V4L2_PIX_FMT_NV21;
	if (ioctl(fd, VIDIOC_S_FMT, &format) == -1) {
			LOGV("set tvin image format failed\n");
			return -1 ;
	}
	
	LOGV("debug width= %d, height =[%d]\n",	format.fmt.pix.width, format.fmt.pix.height);	
	
	if ((720 == format.fmt.pix.width) && (480 == format.fmt.pix.height))
			return TVD_NTSC;

	if ((720 == format.fmt.pix.width) && (576 == format.fmt.pix.height))
		return  TVD_PAL ;
	return TVD_YPBPR;

	

}

void V4L2CameraDevice::setVolatileCtrl(int type, int value)
{
	struct v4l2_queryctrl queryctrl; 
	struct v4l2_control control; 

	memset (&queryctrl, 0, sizeof (queryctrl)); 
	queryctrl.id = type; 
#if 1
	memset (&control, 0, sizeof (control)); 
	control.id = type; 
	control.value = value; 
	if (-1 == ioctl (mCameraFd, VIDIOC_S_CTRL, &control))
	{ 
	 	LOGE("set setVolatileCtrl type:0x%lx error!,error=%s",type,strerror(errno));
		return; 
	}
	ALOGE("FENGYUN SetVolatilectrl ok!!!!!!!!!!!");
#else
	if (ioctl (mCameraFd, VIDIOC_QUERYCTRL, &queryctrl) > 0)
	{ 
		if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
		{
			memset (&control, 0, sizeof (control)); 
			control.id = type; 
			control.value = value; 
			if (-1 == ioctl (mCameraFd, VIDIOC_S_CTRL, &control))
			{ 
			 	LOGE("set setVolatileCtrl type:0x%lx error!",type);
				return; 
			}
		}		
	}
	else 
	{
		LOGW("set VIDIOC_QUERYCTRL is not supported!");
	}
#endif
}

void V4L2CameraDevice::setAnalogInputColor(int brightness,int contrast,int saturation)
{
	LOGV("setAnalogInputColor brightness:%d contrast:%d saturation:%d",brightness,contrast,saturation);
    setVolatileCtrl(V4L2_CID_BRIGHTNESS,brightness);
	setVolatileCtrl(V4L2_CID_CONTRAST,contrast);
    setVolatileCtrl(V4L2_CID_SATURATION,saturation);
}

int V4L2CameraDevice::getCameraType()
{
	return mCameraType;
}

#ifdef SUPPORT_VIRTUAL_CAMERA
status_t V4L2CameraDevice::receiveDataFromCsiCamera(V4L2BUF_t *buf)
{
    CamMessage msg;

    if (read(mCsiFdsC2V[0], &msg, sizeof(CamMessage)) < 0) {
        LOGE("<F:%s, L:%d> read error!", __FUNCTION__, __LINE__);
        return UNKNOWN_ERROR;
    }

    if (msg.msgId == CAM_MESSAGE_TYPE_V4L2DATA) {
        if (read(mCsiFdsC2V[0], buf, sizeof(V4L2BUF_t)) < 0) {
            LOGE("<F:%s, L:%d> read error!", __FUNCTION__, __LINE__);
            return UNKNOWN_ERROR;
        }
    } else {
        LOGE("<F:%s, L:%d> unsupport message!", __FUNCTION__, __LINE__);
        return UNKNOWN_ERROR;
    }
    //ALOGD("<F:%s, L:%d> width=%d, height=%d, paddr=%p", __FUNCTION__, __LINE__, buf->width, buf->height, (char*)buf->addrPhyY);
    return NO_ERROR;
}

status_t V4L2CameraDevice::sendDataToVirCamera(V4L2BUF_t *buf)
{
    CamMessage msg;

    msg.msgId = CAM_MESSAGE_TYPE_V4L2DATA;
    msg.params[0] = 0;
    msg.params[1] = 0;
    msg.params[2] = 0;
    msg.params[3] = 0;

    if (write(mCsiFdsC2V[1], &msg, sizeof(CamMessage)) < 0) {
        LOGE("<F:%s, L:%d> write error!", __FUNCTION__, __LINE__);
        return UNKNOWN_ERROR;
    }
    //ALOGD("<F:%s, L:%d> width=%d, height=%d, paddr=%p", __FUNCTION__, __LINE__, buf->width, buf->height, (char*)buf->addrPhyY);
    if (write(mCsiFdsC2V[1], buf, sizeof(V4L2BUF_t)) < 0) {
        LOGE("<F:%s, L:%d> write error!", __FUNCTION__, __LINE__);
        return UNKNOWN_ERROR;
    }
    return NO_ERROR;
}
#endif

#if 1
//def CAMERA_MANAGER_ENABLE
void V4L2CameraDevice::setCameraManager(CameraManager * manager)
{
	mCameraManager = manager;
	ALOGD("setCameraManager manager=0x%x",manager);
}

void V4L2CameraDevice::startPreview_d()
{
	if((mCameraManager != NULL) && (mCameraManager->isOviewEnable()) 
		&& (mHalCameraInfo.device_id == TVD_DEV_ID_START))
	{
		mCameraManager->startPreview();
	}
}

void V4L2CameraDevice::stopPreview_d()
{
	if((mCameraManager != NULL) && (mCameraManager->isOviewEnable()) 
		&& (mHalCameraInfo.device_id == TVD_DEV_ID_START))
	{
		mCameraManager->stopPreview();
	}
}

void V4L2CameraDevice::releaseCamera_d()
{
	if((mCameraManager != NULL) && (mCameraManager->isOviewEnable()) 
		&& (mHalCameraInfo.device_id == TVD_DEV_ID_START))
	{
		mCameraManager->releaseCamera();
	}
}

#endif

}; /* namespace android */
