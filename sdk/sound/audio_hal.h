#ifndef __AUDIO_HAL_H__
#define __AUDIO_HAL_H__
#include <ui/Rect.h>
#include <fcntl.h>
#include <utils/Thread.h>

#include <linux/videodev2.h>
#include<sys/stat.h>


#include "asoundlib.h"
#include "sdklog.h"
#define F_LOG ALOGV("%s, line: %d", __FUNCTION__, __LINE__);

using namespace android;
typedef void (*audiodata_callback)(int32_t msgType,nsecs_t timestamp,int card,int device,
                                   char *dataPtr,int dsize,
                                   void* user);
enum AudioCapThreadState
{
    /* Do not capture frame. */
    AUDIO_CAP_STATE_NULL,
    AUDIO_CAP_STATE_INIT,
    /* Do not capture frame. */
    AUDIO_CAP_STATE_PAUSED,
    /* Start capture frame. */
    AUDIO_CAP_STATE_STARTED,
    /* exit thread*/
    AUDIO_CAP_STATE_EXIT,
};


#define MAX_AUDIO_DATA_CB_NUM 4
#define AUDIO_SINK_STAT_OPEN 1
#define AUDIO_SINK_STAT_OPEN_MUTE 2

#define AUDIO_SINK_STAT_CLOSE 0
typedef struct AudioSouce
{
    audiodata_callback mDataCb;
    void *mUsrDat;
    int cbStat;
    int isUsed;

} AudioSouce_t;

class AudioCapThread : public Thread
{
    AudioSouce_t AudioSrc[MAX_AUDIO_DATA_CB_NUM];
    int mNewCbCnt;
    int64_t mCurFrameTimestamp;

    int refCnt;
    bool mRequestExit;

public:

    AudioCapThread(int device,int card)
        : Thread(false), mRequestExit(false)
    {
        mNewCbCnt=0;
        refCnt=0;
        buffer_zero =NULL;
        mCurFrameTimestamp=0;
        int i;
        for(i=0; i<MAX_AUDIO_DATA_CB_NUM; i++)
        {
            AudioSrc[i].mDataCb=0;
            AudioSrc[i].mUsrDat=0;
            AudioSrc[i].cbStat=AUDIO_SINK_STAT_CLOSE;
            AudioSrc[i].isUsed=0;
        }
        pthread_mutex_init(&mAudioCapMutex, NULL);
        pthread_cond_init(&mAudioCapCond, NULL);
        mAudioCapThreadState=AUDIO_CAP_STATE_NULL;
        startThread();

    }

    AudioCapThread()
        : Thread(false), mRequestExit(false)
    {
        mNewCbCnt=0;
        refCnt=0;
        int i;
        for(i=0; i<MAX_AUDIO_DATA_CB_NUM; i++)
        {
            AudioSrc[i].mDataCb=0;
            AudioSrc[i].mUsrDat=0;
            AudioSrc[i].cbStat=AUDIO_SINK_STAT_CLOSE;
            AudioSrc[i].isUsed=0;
        }
        pthread_mutex_init(&mAudioCapMutex, NULL);
        pthread_cond_init(&mAudioCapCond, NULL);
        mAudioCapThreadState=AUDIO_CAP_STATE_NULL;
        startThread();

    }
    ~AudioCapThread()
    {


    }
    void stopThread()
    {
        mRequestExit = true;
    }

    void startThread()
    {
        run("AudioCapThread", PRIORITY_URGENT_DISPLAY);
    }
    bool AutAudioCapThread();
    int reqAudioSource(audiodata_callback cb,void *UsrDat);
    int releaseAudioSource(int hdl);
    struct pcm *pcm;
    struct pcm_config config;
    char *buffer;
    char *buffer_zero;


    unsigned int buf_size;
    unsigned int bytes_read ;

    virtual bool threadLoop()
    {
        if (mRequestExit)
        {
            return false;
        }

        return AutAudioCapThread();
    }
    int setCaptureStat(int hdl,int stat)
    {
        F_LOG;
        ALOGV("audio startCapture hdl=%d",hdl);
        if(hdl>=MAX_AUDIO_DATA_CB_NUM)
        {
            ALOGE("hdl=%d>MAX_AUDIO_DATA_CB_NUM",hdl);
            return -1;
        }

        if(stat>AUDIO_SINK_STAT_OPEN_MUTE)
        {
            ALOGE("stat=%d>AUDIO_SINK_STAT_OPEN_MUTE stat=%d",hdl,stat);
            return -1;
        }

        if(AudioSrc[hdl].mDataCb==NULL)
        {
            ALOGE("hdl=%d mDataCb NULL",hdl);
            return -1;
        }

        if(mAudioCapThreadState==AUDIO_CAP_STATE_NULL)
        {
            ALOGE("hdl=%d mAudioCapThreadState NULL",hdl);
            return -1;
        }

        AudioSrc[hdl].cbStat=stat;

    }

    int startCapture(int hdl)
    {
        F_LOG;
        ALOGV("audio startCapture hdl=%d",hdl);
        if(hdl>=MAX_AUDIO_DATA_CB_NUM)
        {
            ALOGE("hdl=%d>MAX_AUDIO_DATA_CB_NUM",hdl);
            return -1;
        }
        if(AudioSrc[hdl].mDataCb==NULL)
        {
            ALOGE("hdl=%d mDataCb NULL",hdl);
            return -1;
        }

        if(mAudioCapThreadState==AUDIO_CAP_STATE_NULL)
        {
            ALOGE("hdl=%d mAudioCapThreadState NULL",hdl);
            return -1;
        }
        //default is open
        //but if setcapturestat other mode etc.AUDIO_SINK_STAT_OPEN_mute before this function ,
        //we psuh zero data to audio endode queue
        if(AudioSrc[hdl].cbStat==AUDIO_SINK_STAT_CLOSE)
            AudioSrc[hdl].cbStat=AUDIO_SINK_STAT_OPEN;
        else
            ALOGW("now audio cap mode =%d",AudioSrc[hdl].cbStat);
        refCnt++;

        if((mAudioCapThreadState==AUDIO_CAP_STATE_INIT)||(mAudioCapThreadState==AUDIO_CAP_STATE_PAUSED))
        {
            pthread_mutex_lock(&mAudioCapMutex);
            mAudioCapThreadState=AUDIO_CAP_STATE_STARTED;
            mCurFrameTimestamp = (int64_t)systemTime();
            // singal to start capture thread
            pthread_cond_signal(&mAudioCapCond);
            pthread_mutex_unlock(&mAudioCapMutex);
        }

        return 0;

    }

    int stopCapture(int hdl)
    {
        F_LOG;
        if(hdl>=MAX_AUDIO_DATA_CB_NUM)
        {
            ALOGE("stopCapture over MAX hdl=%d ",hdl);
            return -1;
        }
        refCnt--;
        AudioSrc[hdl].cbStat=AUDIO_SINK_STAT_CLOSE;
        if(refCnt<=0)
        {
            if(mAudioCapThreadState==AUDIO_CAP_STATE_STARTED)
            {
                pthread_mutex_lock(&mAudioCapMutex);
                mAudioCapThreadState=AUDIO_CAP_STATE_PAUSED;
                // singal to start capture thread
                pthread_mutex_unlock(&mAudioCapMutex);
            }
        }

    }
    int stopAudio()
    {
        if((mAudioCapThreadState==AUDIO_CAP_STATE_PAUSED)&&(mAudioCapThreadState==AUDIO_CAP_STATE_INIT))
        {
            stopThread();
            pthread_mutex_lock(&mAudioCapMutex);
            mAudioCapThreadState=AUDIO_CAP_STATE_EXIT;
            // singal to start capture thread
            pthread_cond_signal(&mAudioCapCond);
            pthread_mutex_unlock(&mAudioCapMutex);
        }
        //for force quit condition
        if(mAudioCapThreadState==AUDIO_CAP_STATE_STARTED)
        {
            stopThread();

            if(buffer)
            {
                free(buffer);
                buffer=NULL;
            }

            if(pcm)
            {
                pcm_close(pcm);
                pcm=NULL;
            }
        }
    }
private:
    enum AudioCapThreadState mAudioCapThreadState;

    pthread_mutex_t 				mAudioCapMutex;
    pthread_cond_t					mAudioCapCond;

};
#endif
