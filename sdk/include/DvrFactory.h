#ifndef __DVRFACTORY_H__
#define __DVRFACTORY_H__

#include "aw_ini_parser.h"

#include <sdklog.h>

#include "CameraDebug.h"

#define USE_CDX_LOWLEVEL_API 1
#define USE_CDX_LOWLEVEL_API_AUDIO 1
#define USE_RECORD_AUDIO_API 1
#include "msgq.h"

#include "V4L2CameraDevice2.h"
#include "CallbackNotifier.h"
#include "PreviewWindow.h"
#include "CameraHardware2.h"
#include "HALCameraFactory.h"
#include "CameraHardwareInterface.h"
#ifdef REC_ENCODE
#include "awrecorder.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#include <sys/time.h>
#ifndef CDX_V27
#include "log.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vencoder.h"
#include "CdxMuxer.h"
#include "CdxParser.h"

#include <time.h>
#include <g2d_driver.h>

#include "Rtc.h"
#include "StorageManager.h"
#include "MuxerWriter.h"
#ifdef USE_RECORD_AUDIO_API
#include "audio_hal.h"
#endif
using namespace android;

//namespace android {

#define MSG_R 0400 /* read permission */
#define MSG_W 0200 /* write permission */
#define SVMSG_MODE (MSG_R | MSG_W | MSG_R >>3 | MSG_R >>6)
#define MQ_KEY  1234L
#define MSGMAX  (256)

#define SECTION_CAMERA_PREFIX "camera"

#define SECTION_CAMERA0 "camera0"
#define SECTION_CAMERA1 "camera1"
#define SECTION_CAMERA360 "camera360"
//#define RECORD_ALONE 1   //dot use ,not full test

//message structure
struct mymesg
{
    long mesg_len;
    long mesg_type;
    nsecs_t timestamp;
    void *user;
    char mesg_data[MSGMAX];
	int iskeyframe;
	int resv;
	char *extradata;
};
typedef struct miscmsg
{
	char *msgdata;
	int len;
	int w;
	int h;
}miscmsg_t;

typedef struct
{
    char			intput_file[256];
    char			output_file[256];
    char            stream_file[256];
	char            mp4_file[256];
	unsigned int  encode_frame_num;
	unsigned int  encode_format;

	unsigned int src_size;
	unsigned int dst_size;

	unsigned int src_width;
	unsigned int src_height;
	unsigned int dst_width;
	unsigned int dst_height;

	unsigned int bit_rate;
	int frame_rate;
	int maxKeyFrame;

	unsigned char write_unmux;
	int muxer_type;
	long long pts;
}encode_param_t;

enum RecordThreadState
{
    /* Do not capture frame. */
    RECORD_STATE_NULL,
    /* Do not capture frame. */
    RECORD_STATE_PAUSED,
    /* Start capture frame. */
    RECORD_STATE_STARTED,
    /* exit thread*/
    RECORD_STATE_EXIT,
};
#define CM_SAVE_FILE_MARK "save"
#define CM_LOCK_FILE_MARK "lock"
#define CM_MAX_RECORDFILE_NUM 200
#define CM_MAX_FILE_LEN 1024
#define CM_THUMB_DIR ".thumb/"
#define CAMERA_MSG_DVR_STORE_ERR  0x40000
#define CAMERA_MSG_DVR_NEW_FILE  0x80000

#define MAX_SUPPORT_CAM 2
typedef struct file_status
{
    int cur_file_idx;
    int cur_max_filenum;

    int timeout_second;
    char cur_filename[CM_MAX_RECORDFILE_NUM][CM_MAX_FILE_LEN];
	char cur_file_thumbname[CM_MAX_RECORDFILE_NUM][CM_MAX_FILE_LEN];//temp no use
    char cur_filedir[MAX_SUPPORT_CAM][CM_MAX_FILE_LEN];
    int cur_dir_file_num;


} file_status_t;

typedef status_t (*usr_data_cb)(int32_t msgType,
                                char *dataPtr,int dalen,
                                void *user);


#define ENC_RINGBUF_LEVEL 16
#define ENC_RINGBUF_DATALEN (300<<10)
typedef struct ringbufQ
{
	CdxMuxerPacketT     	bufhead[ENC_RINGBUF_LEVEL];
	int						write_id;
	int						read_id;
	int                     read_id2;  //for muti rtsp client now support only one
	int						buf_unused;
} ringbufQ_t;

int init_ringbuf(ringbufQ_t *rbuf);
int destory_ringbuf(ringbufQ_t *rbuf);
CdxMuxerPacketT * push_ringbuf(ringbufQ_t *rbuf);
CdxMuxerPacketT * pop_ringbuf(ringbufQ_t *rbuf);

typedef struct h264_encoder_parm
{
	int bitrate;
	int in_width;
	int in_height;
	int out_width;
	int out_height;
	int framerate;
	VENC_PIXEL_FMT  	eInputFormat;
	long long pts;
}h264_encoder_parm_t;

#define CAMERA_NAME_LEN 16

#define CurRecordFileNameType_NORMAL 1
#define CurRecordFileNameType_LOCK 2
#define CurRecordFileNameType_SAVE 3

#define RecordStat_NORMAL 1
#define RecordStat_STOPCAR 2


#define LockOps_CreateNewNow 1
#define LockOps_CreateNewAfterClose 2

#define MISCWORK_CMD_RM 0XFF01
#define MISCWORK_CMD_SAVETHUMB 0XFF02
#define RECORD_CMD_TIMEOUT 0XFF03
#define RECORD_CMD_VIDEO 0XFF04
#define RECORD_CMD_AUDIO 0XFF05



#define G2D_FUNC_SCALE_WIDTH  640
#define G2D_FUNC_SCALE_HEIGHT 480
#define G2D_FUNC_BUFFER_SIZE (1920*1080*3/2)

#define CMD_MP_SCALE 1

#define SMART_RECORD 1

class dvr_factory
{

public:
	void set_rtsp_to_file(int yest)
	{
		istestrtsp =yest;

	}
	
//#ifdef USE_MP_CONVERT
	int 							mG2DHandle;
	unsigned int mG2dAddrVir;
	unsigned int mG2dAddrPhy;
	struct ScMemOpsS* mCameraMemops;
	
	int genThumbData(char* dataPtr,V4L2BUF_t *outbuf);
	
	int g2d_func_open();
	int g2d_func_close();
	int g2d_func_scale(unsigned char *src, int src_width, int src_height, unsigned char *dst, int dst_width, int dst_height);
//#endif
    dvr_factory();
    ~dvr_factory();
    int recordInit(const char *name);

    int prepare();
    int start();
    int stop();
	#ifdef USE_CDX_LOWLEVEL_API_AUDIO
	int AudioRecStart();
	int AudioRecStop();
	#endif
    bool initializeDev(const char *name);
    bool uninitializeDev();
	int startRecord();
	int stopRecord();

    int SetDataCB(usr_data_cb cb,void* user)
    {
        usrDatCb=cb;
        mCbUserDat=user;
        return 0;
    }
	int start_rtsp();
	int stop_rtsp();
	int rtsp_started;
	ringbufQ_t rtspQbuf;
	VencHeaderData rtsp_sps_pps;
	Mutex    mRtspLock;
	int mCameraId;
//	char cameraname[CAMERA_NAME_LEN];
	int generateThumbFile();
	int mNeedThumbpic;
	int start_deliverrtsp(int disp_width,int disp_height);
	int stop_deliverrtsp();
	int can_deliver_rtspdata;
	h264_encoder_parm_t rtsp_encparm;

	int create_rtsp_encoder(int bitrate,int in_width,int in_height,int out_width,int	out_height,int framerate);
	int encode_rtsp(V4L2BUF_t *pdata,char*outbuf,int*outsize,void* user,int freebuf);
	int destory_rtsp_encoder();
	
    void setCallbacks(notify_callback notify_cb,
                      data_callback data_cb,
                      data_callback_timestamp data_cb_timestamp,
                      void* user);

    int startPriview(struct view_info vv);
    int stopPriview();
    int takePicture();
	int setCrop(int newzoom,int maxzoom);
    bool initFileListDir( char * dirname );
    int allocFileSpace();
    int encode(V4L2BUF_t *pdata,char*outbuf,int*outsize,void* user);
    int enc_de_init();

    int setDuration(int sec);
    int getDuration();
    int  genfilename(char* name);

    int closemux();
    int enc_parm_init(int sw,int sh,int dw,int dh,unsigned int bitrate,int framerate,VENC_CODEC_TYPE type);

    int recordwith;
    int recordheith;
    //water mark
    bool enableWaterMark();
    bool disableWaterMark();
    status_t setWaterMarkMultiple(const char *str);
	#ifdef ADAS_ENABLE
	bool enableAdas();
	bool disableAdas();
	status_t setAdasParameters(int key,int value);
	bool setAdasCallbacks(adas_data_callback adas_data_cb);
	#endif
	int storage_state;
	int setCurRecordFileLock();
	int setCurRecordFileSave();
	int setCurRecordFileNormal();
	//for 1 :normal for 2:stop car record
	int setCurRecordStat(int stat);

	int setCurRecordLockOps(int ops);
	//#if FS_WRITER
    CdxFsCacheMemInfo fs_cache_mem;
    int fs_mode;// = FSWRITEMODE_DIRECT;
//#endif
#ifdef USE_RECORD_AUDIO_API
	static void __audio_data_cb_timestamp(int32_t msgType,nsecs_t timestamp,int card,int device,
                            char *dataPtr,int dsize,
                            void* user)
	{
	    if(user==NULL)
	    {
	        LOGW("__audio_data_cb_timestamp user null");
	        return ;
	    }

		 //cmd queue
		struct mymesg *mesg=(struct mymesg *)malloc(sizeof(struct mymesg));
		 
	    if(mesg==NULL)
	    {
	        LOGW("__audio_data_cb_timestamp mesg fail");
	        return ;
	    }
		mesg->extradata=(char *)malloc(dsize);
		if(mesg->extradata==NULL)
	    {
	        LOGW("__audio_data_cb_timestamp extradata fail");
			free(mesg);
			mesg = NULL;
	        return ;
	    }
	    memcpy(mesg->extradata,dataPtr,dsize);//blank
	    mesg->timestamp = timestamp;
	    mesg->user=user;
	    mesg->mesg_len = dsize;
	    mesg->mesg_type = msgType;
		//ALOGV("--__audio_data_cb_timestamp----PUSH---%p",mesg);
		int ret = OSAL_Queue(&((dvr_factory*)user)->mQueueBufferAudioEnc, mesg);
		if (ret != 0)
		{
			ALOGV("--__audio_data_cb_timestamp----OSAL_Queue---%p fail,numElem",mesg,((dvr_factory*)user)->mQueueBufferAudioEnc.numElem);
			free(mesg->extradata);
			mesg->extradata = NULL;
			free(mesg);
			mesg = NULL;
			return ;
		}
		pthread_mutex_lock(&((dvr_factory*)user)->mAudioEncMutex);
		pthread_cond_signal(&((dvr_factory*)user)->mAudioEncCond);
    	pthread_mutex_unlock(&((dvr_factory*)user)->mAudioEncMutex);
		return;

	}
	sp<AudioCapThread> mAudioCap;
	int mAudioHdl;
	int setAudioOps(sp<AudioCapThread> act,int hdl){
		ALOGV("----setAudioOps----hdl=%d-----",hdl);
		mAudioCap=act;
		mAudioHdl=hdl;
		
		return 0;
	}
#endif
	int savePicture(void* data, int size, int fd);
	int sendCmd2MiscWork(int cmd,void *data,int len);
private:
	int mCurRecordStat;
	int mCurRecordFileNameType;
	int mCurRecordLockOps;
	int camera_video_file_save(char *name,int filetype);
	int camera_change_file_name(int filetype);
	

    int generatepicFile();
	int mDvrWriteFrameNum;
	int mAudioflag;
	class MiscWorkThread : public Thread
	{
		dvr_factory* cam;
		bool mRequestExit;
	public:
	
		MiscWorkThread(dvr_factory* hw)
			: Thread(false), cam(hw),mRequestExit(false) { }
	
		void stopThread()
		{
			mRequestExit = true;
		}
	
		void startThread()
		{
			run("DvrMiscWorkThread", PRIORITY_URGENT_DISPLAY);
		}
	
		virtual bool threadLoop()
		{
			if (mRequestExit)
			{
	
				return false;
			}
	
			return cam->DvrMiscWorkThread();
		}
	};
	
	bool DvrMiscWorkThread();
	enum RecordThreadState mMiscWorkThreadState;
    sp<MiscWorkThread>			mMiscWorkThread;
    pthread_mutex_t 				mMiscWorkMutex;
    pthread_cond_t					mMiscWorkCond;
    OSAL_QUEUE						mQueueBufferMiscWork; //cmd queue

	
    class RecordFileThread : public Thread
    {
        dvr_factory* cam;
        bool mRequestExit;
    public:

        RecordFileThread(dvr_factory* hw)
            : Thread(false), cam(hw),mRequestExit(false) { }

        void stopThread()
        {
            mRequestExit = true;
        }

        void startThread()
        {
            run("CameraRecordFileThread", PRIORITY_URGENT_DISPLAY);
        }

        virtual bool threadLoop()
        {
            if (mRequestExit)
            {

                return false;
            }

            return cam->recordThread();
        }
    };
    bool recordThread();

    int awrecordwrite(struct mymesg  * pmsg);
    int setparm();

    int dvrsetvideoname(char* name);

    static void notifyCallback(int32_t msgType, int32_t ext1,
                               int32_t ext2, void* user)
    {
    ALOGV("dvr factory %s", __FUNCTION__);
#ifdef REC_ENCODE
        AwRecorder* 	  mAwRecorder;
#endif
    }
    static void __notify_cb(int32_t msg_type, int32_t ext1,
                            int32_t ext2, void *user)
    {
        ALOGV("dvr factory %s", __FUNCTION__);
        dvr_factory *__this =
            static_cast<dvr_factory *>(user);
        __this->notifyCallback(msg_type, ext1, ext2, user);
        if(__this->mNotifyCb!=NULL){
			ALOGV("dvr factory %s line=%d cbdat=%p", __FUNCTION__,__LINE__,__this->mCbUser);
            __this->mNotifyCb(msg_type, ext1, ext2, __this->mCbUser);
        	}
    }
    static void __data_cb(int32_t msg_type,
                          char *data,
                          camera_frame_metadata_t *metadata,
                          void *user)
    {
        //ALOGV("%s", __FUNCTION__);
        dvr_factory *__this =
            static_cast<dvr_factory *>(user);
        __this->dataCallback(msg_type,data,metadata,user);
        if(__this->mDataCb!=NULL)
            __this->mDataCb(msg_type,data,metadata,__this->mCbUser);

    }

    static void __data_cb_timestamp(nsecs_t timestamp, int32_t msg_type,
                                   char *data,
                                   void *user)
    {
        //ALOGV("%s", __FUNCTION__);
        dvr_factory *__this =
            static_cast<dvr_factory *>(user);
		//ALOGE("--------------data=%p",data);
		
        if(__this->mDataCbTimestamp!=NULL)
            __this->mDataCbTimestamp(timestamp, msg_type, data, __this->mCbUser);

        __this->dataCallbackTimestamp(timestamp, msg_type, data, user);

    }
	long long FileNewStampBase;
	int mNeedNewFile;
	int mCurRecordPos;
	int mTimeridVaild;
	timer_t  mTimerID;
    static void __timer_cb(sigval_t sig);


    void dataCallback(int32_t msgType,
                      char* dataPtr, camera_frame_metadata_t *metadata, void* user);

    status_t dataCallbackTimestamp(nsecs_t timestamp,
                                   int32_t msgType, char* dataPtr, void* user) ;
	#ifdef USE_CDX_LOWLEVEL_API_AUDIO
	int AutAudioEncInit(AudioEncConfig* pconfig);
	int AutAudioEncDeinit();
	bool AutAudioEncThread();
	int AutAudioMuxInit(CdxMuxerMediaInfoT *pmedia_info);
	class AudioEncThread : public Thread
	{
		dvr_factory* cam;
		bool mRequestExit;
	public:
	
		AudioEncThread(dvr_factory* hw)
			: Thread(false), cam(hw),mRequestExit(false) { }
	
		void stopThread()
		{
			mRequestExit = true;
		}
	
		void startThread()
		{
			run("AudioEncThread", PRIORITY_URGENT_DISPLAY);
		}
	
		virtual bool threadLoop()
		{
			if (mRequestExit)
			{
				return false;
			}
	
			return cam->AutAudioEncThread();
		}
	};
	enum RecordThreadState mAudioThreadState;
    sp<AudioEncThread>			mAudioEncThread;
    pthread_mutex_t 				mAudioEncMutex;
    pthread_cond_t					mAudioEncCond;
    OSAL_QUEUE						mQueueBufferAudioEnc; //cmd queue

	#endif

protected:
    /* Locks this instance for parameters, state, etc. change. */
    Mutex                           mObjectLock;
public:
    int setGpsData(void *gpsData)
    {
        return mHardware->sendCommand(CAMERA_CMD_SET_GPSDATA_RECORDER, (int32_t)gpsData, 0);
    }
	int mUseMsgQ;
    int setWhiteBlanceData(int32_t Data)
    {
		return mHardware->sendCommand(CAMERA_CMD_SET_WHITE_BALANCE, Data, 0);
    }

	int setColorEffectData(int32_t Data)
    {
		return mHardware->sendCommand(CAMERA_CMD_SET_COLOR_EFFECT, Data, 0);
    }

	int setExposureCompensationData(int32_t Data)
    {
		return mHardware->sendCommand(CAMERA_CMD_SET_EXPOSURE_COMPENSATION, Data, 0);
    }
	int mVideoRecordQueueErr;
	int mAudioRecordQueueErr;
private:
	MuxerWriterT *mw;
    CdxWriterT *writer;
	VencBaseConfig rec_enc_baseConfig;
	VencBaseConfig rtsp_enc_baseConfig;
	int mCaptureFmt;
    int callbackData;

    notify_callback         mNotifyCb;
    data_callback           mDataCb;
    data_callback_timestamp mDataCbTimestamp;
    void *mCbUser;

    //record threads
    enum RecordThreadState dvrstatus;
    sp<RecordFileThread>			mRecordFileThread;
    pthread_mutex_t 				mRecordFileMutex;
    pthread_cond_t					mRecordFileCond;
    OSAL_QUEUE						mQueueBufferRecordFile;
	msgQ_t	mQueueRecordFile;
	
    sp<CameraHardwareInterface>     mHardware;

    HwDisplay* mcd;

    //h264 data cb
    usr_data_cb usrDatCb;
    void *mCbUserDat;

    //test
    FILE *out_file ;
	FILE *out_filertsp ;

	int istestrtsp;

    //encode
    encode_param_t	encode_param;
    VideoEncoder* pVideoEnc;
	VideoEncoder* pVideoEncRtsp;
	
	AudioEncoder*	pAudioEncode;
	AudioEncConfig mAudioEncConfig;

    //muxer
    CdxMuxerT *mux;
    CdxDataSourceT mp4stream;
    pthread_mutex_t mMutex;

    int mDuration;
    file_status_t mFilemanger;
	int mRecordInitDone;
};

//}

#endif
