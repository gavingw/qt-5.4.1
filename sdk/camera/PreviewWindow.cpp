
#include "CameraDebug.h"
#if DBG_PREVIEW
#define LOG_NDEBUG 0
#endif
#define LOG_TAG "PreviewWindow"
#include <cutils/log.h>

#include <ui/Rect.h>
//#include <ui/GraphicBufferMapper.h>

#include "V4L2CameraDevice2.h"
#include "PreviewWindow.h"

#ifndef DMAFD_TEST
//使用硬件图层显示视频的每一帧
#define USE_HW_LAYER   1
#endif

#ifdef DMAFD_TEST
#include "DmaIon.h"
extern "C" int info_aframe(int *dmafd);
#endif

namespace android {

/*#define CAMHAL_GRALLOC_USAGE GRALLOC_USAGE_HW_TEXTURE | \
                                 GRALLOC_USAGE_HW_RENDER | \
                                 GRALLOC_USAGE_SW_READ_RARELY | \
                                 GRALLOC_USAGE_SW_WRITE_NEVER
*/

#define CAMHAL_GRALLOC_USAGE GRALLOC_USAGE_SW_READ_RARELY | \
                                     GRALLOC_USAGE_SW_WRITE_OFTEN
#define USE_NEW_AUT_DISP 1
static int calculateFrameSize(int width, int height, uint32_t pix_fmt)
{
    int frame_size = 0;
    switch (pix_fmt) {
        case V4L2_PIX_FMT_YVU420:
        case V4L2_PIX_FMT_YUV420:
        case V4L2_PIX_FMT_NV21:
        case V4L2_PIX_FMT_NV12:
            frame_size = (ALIGN_16B(width) * height * 12) / 8;
            break;
        case V4L2_PIX_FMT_YUYV:
            frame_size = (width * height) << 1;
            break;
        default:
            ALOGE("%s: Unknown pixel format %d(%.4s)",
                __FUNCTION__, pix_fmt, reinterpret_cast<const char*>(&pix_fmt));
            break;
    }
    return frame_size;
}

DBG_TIME_AVG_BEGIN(TAG_CPY);
DBG_TIME_AVG_BEGIN(TAG_DQBUF);
DBG_TIME_AVG_BEGIN(TAG_LKBUF);
DBG_TIME_AVG_BEGIN(TAG_MAPPER);
DBG_TIME_AVG_BEGIN(TAG_EQBUF);
DBG_TIME_AVG_BEGIN(TAG_UNLKBUF);

PreviewWindow::PreviewWindow():
//    : mPreviewWindow(NULL)
      mPreviewFrameWidth(0),
      mPreviewFrameHeight(0),
      mPreviewFrameSize(0),
      mCurPixelFormat(0),
      mPreviewEnabled(false),
      mShouldAdjustDimensions(true)
      ,isPreviewDispEn(false)
{
	F_LOG;
	mHwDisplay = HwDisplay::getInstance();
	mPreviewMemops = MemAdapterGetOpsS();	 
	if(mPreviewMemops == NULL)	  { LOGE("camera MemAdapterGetOpsS failed\n");		  return;}    
	memset(&imgtr,0,sizeof(tr_obj));
	int ret = CdcMemOpen(mPreviewMemops);

	if (ret < 0)
	{
		LOGE("ion_alloc_open failed");
	}
	int i;
	for(i=0;i<DISP_NB_BUFFER;i++){
		mDispBuff[i].addrPhyY = 0;
		mDispBuff[i].addrVirY = 0;
	}
}

PreviewWindow::~PreviewWindow()
{
	F_LOG;

	if(mPreviewMemops != NULL)
		CdcMemClose(mPreviewMemops);

	mPreviewMemops=NULL;
}

/****************************************************************************
 * Camera API
 ***************************************************************************/
#if 0
status_t PreviewWindow::setPreviewWindow(struct preview_stream_ops* window)
{
    LOGV("%s: current: %p -> new: %p", __FUNCTION__, mPreviewWindow, window);

    status_t res = NO_ERROR;
    Mutex::Autolock locker(&mObjectLock);

    /* Reset preview info. */
    mPreviewFrameWidth = mPreviewFrameHeight = 0;

    if (window != NULL) {
        /* The CPU will write each frame to the preview window buffer.
         * Note that we delay setting preview window buffer geometry until
         * frames start to come in. */
        res = window->set_usage(window, /*GRALLOC_USAGE_SW_WRITE_OFTEN*/CAMHAL_GRALLOC_USAGE);
        if (res != NO_ERROR) {
            window = NULL;
            res = -res; // set_usage returns a negative errno.
            LOGE("%s: Error setting preview window usage %d -> %s",
                 __FUNCTION__, res, strerror(res));
        }
    }
    mPreviewWindow = window;

    return res;
}
#endif

#ifdef DISP_LIB_V20
//	static struct ScMemOpsS* g_dispmemops;
//	static int g_disp_paddr;
#endif

status_t PreviewWindow::setPreviewDisplayOn()
{
    //LOGD("%s: current: %p -> new: %p", __FUNCTION__, mPreviewWindow, window);
	F_LOG

    status_t res = NO_ERROR;
    Mutex::Autolock locker(&mObjectLock);
	isPreviewDispEn=true;

	int dmode = aut_get_disp_rotate_mode();
	if(dmode >0){
		aut_imgtr_init(&imgtr);
		
		int i;
		for(i=0;i<DISP_NB_BUFFER;i++){
			if(mDispBuff[i].addrVirY==NULL){
				mDispBuff[i].addrVirY= (int)CdcMemPalloc(mPreviewMemops, PBUFFER_SIZE);
				mDispBuff[i].addrPhyY= (unsigned int)CdcMemGetPhysicAddressCpu(mPreviewMemops,(void*)mDispBuff[i].addrVirY);
			}
		}
	}
	

#ifdef USE_HW_LAYER
	#ifdef DISP_LIB_V20
		/*
		#define PIC1080_PATH "/tmp/a.yuv"

		#define PIC1080_WIDTH  3840
		#define PIC1080_HIGHT  2160
	
		g_dispmemops = MemAdapterGetOpsS();	 
		int ret = CdcMemOpen(g_dispmemops);
		if (ret < 0)
		{
			printf("ion_alloc_open failed\r\n");
			return -1;
		}
		int disp_vaddr= (int)CdcMemPalloc(g_dispmemops, (PIC1080_WIDTH*PIC1080_HIGHT*3/2));
		g_disp_paddr=(int)CdcMemGetPhysicAddressCpu(g_dispmemops,(void*)disp_vaddr);

		FILE *fpff = fopen(PIC1080_PATH,"rb");
		if(NULL == fpff){
			printf("fopen YUV ERR,return now!!!\r\n");
			return -1;
		}
		
		printf("fopen YUV OK \r\n");
		fread((void *)disp_vaddr, 1, PIC1080_WIDTH*PIC1080_HIGHT*3/2, fpff);
		fclose(fpff);
		struct src_info src={PIC1080_WIDTH,PIC1080_HIGHT,DISP_FORMAT_YUV420_SP_VUVU};
		mHwDisplay->aut_hwd_layer_set_src(0x010000,&src,(int)&g_disp_paddr);
		*/
		
		mHwDisplay->aut_hwd_layer_open(0);
		mHwDisplay->aut_hwd_layer_open(0x010000);
	#else
		mHwDisplay->hwd_layer_set_src(mHlay,&msi);
		struct view_info screenview =mvi;
			
		mHwDisplay->hwd_layer_set_rect(mHlay, &screenview);
		mHwDisplay->hwd_video_start(mHlay);
		if(mHwDisplay->mScreen1Mode>0)
		{

		switch(mHwDisplay->mScreen1Mode)
		{

			case DISP_TV_MOD_PAL:
				screenview.w = 720;
				screenview.h = 576;
				break;
				
			case DISP_TV_MOD_NTSC:
				screenview.w	= 720;
				screenview.h	= 480;			break;
				
			case DISP_TV_MOD_720P_60HZ:
			case DISP_TV_MOD_720P_50HZ:
				screenview.w	= 1280;
				screenview.h	= 720;
				break;
				
			case DISP_TV_MOD_1080I_50HZ:
			case DISP_TV_MOD_1080I_60HZ:
			case DISP_TV_MOD_1080P_24HZ:
			case DISP_TV_MOD_1080P_50HZ:
			case DISP_TV_MOD_1080P_60HZ:
				screenview.w	= 1920;
				screenview.h	= 1080;
				break;

			case DISP_VGA_MOD_640_480P_60:
				screenview.w	= 640;
				screenview.h	= 480;
				break;
				
			case DISP_VGA_MOD_800_600P_60:
				screenview.w	= 800;
				screenview.h	= 600;
				break;
				

			case DISP_VGA_MOD_1024_768P_60:
				screenview.w	= 1024;
				screenview.h	= 768;
				break;
				
			case DISP_VGA_MOD_1280_720P_60:
				screenview.w	= 1280;
				screenview.h	= 720;
				break;
			
				
			default:
				LOGE("error :mHwDisplay->mScreen1Mode=%d didn't fix any thing\n",mHwDisplay->mScreen1Mode);
				break;

		}
		//struct view_info screen1view ={mvi.x,mvi.y,mvi.w/mHwDisplay->lcdxres*720,mvi.h/mHwDisplay->lcdyres*480};
		mHwDisplay->hwd_layer_set_rect(1,S_CHANNEL_ID,S_LAYER_ID, &screenview);
		mHwDisplay->hwd_video_start(1,S_CHANNEL_ID,S_LAYER_ID);
	}
	#endif
#endif

    return res;

}

status_t PreviewWindow::setPreviewParam(struct view_info pvi,struct src_info  psi,bool tvout,int tvch)
{
	mvi=pvi;
	msi=psi;
	mtvout=tvout;
	mtvch=tvch;
}

status_t PreviewWindow::setPreviewDisplayOff()
{
    //LOGD("%s: current: %p -> new: %p", __FUNCTION__, mPreviewWindow, window);
	F_LOG;
    status_t res = NO_ERROR;
    Mutex::Autolock locker(&mObjectLock);
	isPreviewDispEn=false;
#ifdef USE_HW_LAYER
	#ifdef DISP_LIB_V20
		mHwDisplay->aut_hwd_layer_close(0);
		mHwDisplay->aut_hwd_layer_close(0x010000);
	#else
	mHwDisplay->hwd_video_stop(mHlay);
	if(mHwDisplay->mScreen1Mode>0)
	{
		mHwDisplay->hwd_video_stop(1,S_CHANNEL_ID,S_LAYER_ID);
	}
	#endif
#endif
	
	int dmode = aut_get_disp_rotate_mode();
	if(dmode >0){
		aut_imgtr_release(&imgtr);

	}
	//heat so guys change the rotate mode during the preview rtime.
	//so check release the buffer out of dmode domain
	int i;
	for(i=0;i<DISP_NB_BUFFER;i++){
		if(mDispBuff[i].addrVirY != NULL)
		{
			CdcMemPfree(mPreviewMemops,(void*)mDispBuff[i].addrVirY);
			mDispBuff[i].addrPhyY = 0;
			mDispBuff[i].addrVirY = 0;
		}
	}
	mHlay=-1;


    return res;
}

status_t PreviewWindow::startPreview()
{
    F_LOG;

    Mutex::Autolock locker(&mObjectLock);
    mPreviewEnabled = true;

    DBG_TIME_AVG_INIT(TAG_CPY);
    DBG_TIME_AVG_INIT(TAG_DQBUF);
    DBG_TIME_AVG_INIT(TAG_LKBUF);
    DBG_TIME_AVG_INIT(TAG_MAPPER);
    DBG_TIME_AVG_INIT(TAG_EQBUF);
    DBG_TIME_AVG_INIT(TAG_UNLKBUF);
    return NO_ERROR;
}

void PreviewWindow::stopPreview()
{
    F_LOG;

    Mutex::Autolock locker(&mObjectLock);
    mPreviewEnabled = false;
    mShouldAdjustDimensions = true;

    DBG_TIME_AVG_END(TAG_CPY, "copy ");
    DBG_TIME_AVG_END(TAG_DQBUF, "deque ");
    DBG_TIME_AVG_END(TAG_LKBUF, "lock ");
    DBG_TIME_AVG_END(TAG_MAPPER, "mapper ");
    DBG_TIME_AVG_END(TAG_EQBUF, "enque ");
    DBG_TIME_AVG_END(TAG_UNLKBUF, "unlock ");
}
static int preview_saveframe(char *str,void *p,int length,int is_oneframe)
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

/****************************************************************************
 * Public API
 ***************************************************************************/

static int firstxp=0;
bool PreviewWindow::onNextFrameAvailable(const void* frame, int flipEnable,bool isOview)
{
    int res;
    Mutex::Autolock locker(&mObjectLock);
	if(!isPreviewDispEn)
		return true;




//return 0;







	if(firstxp==0)
	{
		firstxp	= 1;
		ALOGE("onNextFrameAvailable------------DONE");
	}
    V4L2BUF_t * pv4l2_buf = (V4L2BUF_t *)frame;

    int preview_format = 0;
    int preview_addr_phy = 0;
    int preview_addr_vir = 0;
    int preview_width = 0;
    int preview_height = 0;
    RECT_t preview_crop;
    t_VideoBufferInfo bufInfo;
	int cur_idx=0;
//F_LOG
    if (!isPreviewEnabled() )//|| mPreviewWindow == NULL)
    {
        return true;
    }
	struct src_info src;
	src.w = pv4l2_buf->width;
	src.h = pv4l2_buf->height;
	cur_idx = pv4l2_buf->index%DISP_NB_BUFFER;
		#if 0
		static int cntt=0;
		char buf[128];
		if((cntt++%30)==0){
			sprintf(buf,"/tmp/%d.nv21",cntt);
			preview_saveframe(buf,(void*)pv4l2_buf->addrVirY,src.w*src.h*3/2,1);
		}
		#endif
	//case lcd 1920x480
	//video 1920x1080 1280x720 use crop
	int cropview_h=src.w*mHwDisplay->lcdyres/mHwDisplay->lcdxres;

//	printf("preview1 crop =[%d , %d, %d ,%d]\r\n",pv4l2_buf->crop_rect.left,pv4l2_buf->crop_rect.top,pv4l2_buf->crop_rect.width,pv4l2_buf->crop_rect.height);
#ifdef ASDK
	struct view_info crop={pv4l2_buf->crop_rect.left,pv4l2_buf->crop_rect.top,pv4l2_buf->crop_rect.width,pv4l2_buf->crop_rect.height};

#else
	struct view_info crop={0,(src.h-(src.w / 4))/2,src.w,(src.w / 4)};

#endif
	
	frm_info_t img;
	int disp_format;
	int ww;
	int hh;
	int addry;
	int addrc;
	int dmode = aut_get_disp_rotate_mode();

	if(dmode >0){
	//rotate 90 180 270 etc.
		img.addr0=&pv4l2_buf->addrPhyY;
		img.addr1 = &pv4l2_buf->addrPhyC;
		img.format = TR_FORMAT_YUV420_SP_VUVU;
		img.width = src.w;
		img.height = src.h;
		img.mode = (tr_mode)dmode;
		img.outaddr0 = &mDispBuff[cur_idx].addrPhyY;
		//img.outaddr1 = &mDispBuff[cur_idx].addrPhyY;
		//if(mPreviewMemops!=NULL)
		//	CdcMemFlushCache(mPreviewMemops,(void*)pv4l2_buf->addrVirY, img.width * img.height *3/ 2);

		aut_imgtr_proc(&imgtr,&img);

		if((img.mode == TR_ROT_180)||(img.mode == TR_HFLIP)||(img.mode == TR_VFLIP) ){
			ww=src.w;
			hh=src.h;
			crop={pv4l2_buf->crop_rect.left,pv4l2_buf->crop_rect.top,pv4l2_buf->crop_rect.width,pv4l2_buf->crop_rect.height};
		}else{
			ww=src.h;
			hh=src.w;
			crop={pv4l2_buf->crop_rect.left,pv4l2_buf->crop_rect.top,pv4l2_buf->crop_rect.height,pv4l2_buf->crop_rect.width};
		}
		
		if(mPreviewMemops!=NULL)
			CdcMemFlushCache(mPreviewMemops,(void*)mDispBuff[cur_idx].addrVirY, ALIGN_32B(ww) * hh *3/ 2);
		#if 1
		static int cnt =0;
		char buf[64];
		cnt++;
		if(cnt/16==0){
			sprintf(buf,"/tmp/%d.yv12",cnt);
			preview_saveframe(buf,(void *)mDispBuff[cur_idx].addrVirY,ALIGN_32B(ww) * hh *3/ 2,1);
		}
		#endif
		disp_format = DISP_FORMAT_YUV420_P;

		addry = mDispBuff[cur_idx].addrPhyY;
		addrc = addry+ALIGN_32B(ww)*hh;
		
	}else{
		//org 
		ww=src.w;
		hh=src.h;
		disp_format = DISP_FORMAT_YUV420_SP_VUVU;

		addry = pv4l2_buf->addrPhyY;
		addrc = pv4l2_buf->addrPhyC;
	}
	
#ifdef USE_HW_LAYER
	#ifdef DISP_LIB_V20
		src.format = disp_format;
		src.w = ww;
		src.h = hh;
		mHwDisplay->aut_hwd_layer_set_rect(0,&crop);
		int ret = mHwDisplay->aut_hwd_layer_set_src(0,&src,(int)&addry);//screen0=0,ch=0,lyr=0 ===>hlay=0x000000
		if(ret < 0){
			printf("onNextFrameAvailable set video0 src error\n");
		}

		mHwDisplay->aut_hwd_layer_set_rect(0x010000,&crop);
		ret = mHwDisplay->aut_hwd_layer_set_src(0x010000,&src,(int)&addry);  //screen0=1,ch=0,lyr=0 ===>hlay=0x010000
		if(ret < 0){
			printf("onNextFrameAvailable:set video1 src error\n");
		}
	#else
		mHwDisplay->hwd_video_set_crop(mHlay,crop);
		mHwDisplay->hwd_video_set_addr(mHlay,disp_format,ww, hh, &addry,&addrc);
		if(mHwDisplay->mScreen1Mode>0){
			mHwDisplay->hwd_video_set_addr(1,S_CHANNEL_ID,S_LAYER_ID,disp_format,ww, hh, &addry,&addrc);
		}
	#endif
#else
	ion_buffer_node *pibuf=(ion_buffer_node *) pv4l2_buf->overlay_info;
	info_aframe(&pibuf->dmabuf_fd);	
	//ALOGV("------xx-------dmafd----=%d",pibuf->dmabuf_fd);
#endif

   return true;
}

/***************************************************************************
 * Private API
 **************************************************************************/

bool PreviewWindow::adjustPreviewDimensions(V4L2BUF_t* pbuf,bool isOview)
{
    /* Match the cached frame dimensions against the actual ones. */
    if ((pbuf->isThumbAvailable == 1)
        && (pbuf->thumbUsedForPreview == 1))        // use thumb frame for preview
    {
        if ((mPreviewFrameWidth == pbuf->thumbWidth)
            && (mPreviewFrameHeight == pbuf->thumbHeight)
            && (mCurPixelFormat == pbuf->thumbFormat))
        {
            /* They match. */
            return false;
        }

        LOGV("cru: [%d, %d], get: [%d, %d]", mPreviewFrameWidth, mPreviewFrameHeight,
            pbuf->thumbWidth, pbuf->thumbHeight);
        /* They don't match: adjust the cache. */
        mPreviewFrameWidth = pbuf->thumbWidth;
        mPreviewFrameHeight = pbuf->thumbHeight;
        mCurPixelFormat = pbuf->thumbFormat;

        mPreviewFrameSize = calculateFrameSize(pbuf->thumbWidth, pbuf->thumbHeight, pbuf->thumbFormat);
    }
    else
    {
		if(isOview)
		{
			if ((mPreviewFrameWidth == BIRDVIEW_OUTPUT_WIDTH)
			&& (mPreviewFrameHeight == BIRDVIEW_OUTPUT_HEIGHT)
			&& (mCurPixelFormat == pbuf->format)) 
			{
				return false;
			}
		}
		else
		{
			if ((mPreviewFrameWidth == pbuf->width)
            && (mPreviewFrameHeight == pbuf->height)
            && (mCurPixelFormat == pbuf->format))
        {
            /* They match. */
            return false;
        }
		}

        LOGV("cru: [%d, %d], get: [%d, %d]", mPreviewFrameWidth, mPreviewFrameHeight,
            pbuf->width, pbuf->height);
        /* They don't match: adjust the cache. */
		if(isOview)
		{
			mPreviewFrameWidth = BIRDVIEW_OUTPUT_WIDTH;
        	mPreviewFrameHeight = BIRDVIEW_OUTPUT_HEIGHT;
		}
        else
        {
        	mPreviewFrameWidth = pbuf->width;
        	mPreviewFrameHeight = pbuf->height;
        }
        mCurPixelFormat = pbuf->format;

        mPreviewFrameSize = calculateFrameSize(pbuf->width, pbuf->height, pbuf->format);
    }

    mShouldAdjustDimensions = false;
    return true;
}

}; /* namespace android */
