#include "audio_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>


int AudioCapThread::reqAudioSource(audiodata_callback cb,void *UsrDat)
{
    int i;
    if(mNewCbCnt+1>=MAX_AUDIO_DATA_CB_NUM)
    {
        ALOGE("%s mNewCbCnt MAX",__FUNCTION__);
        return -1;
    }

    if(cb==NULL)
    {
        ALOGE("%s cb NULL",__FUNCTION__);
        return -1;
    }

    for(i=0; i<MAX_AUDIO_DATA_CB_NUM; i++)
    {
        ALOGV("reqAudioSource idx=%d isuded=%d",i,AudioSrc[i].isUsed);
        if(AudioSrc[i].isUsed==0)
        {
            AudioSrc[i].mDataCb=cb;
            AudioSrc[i].mUsrDat=UsrDat;
            AudioSrc[i].cbStat=AUDIO_SINK_STAT_CLOSE;
            AudioSrc[i].isUsed=1;

            mNewCbCnt++;
            break;
        }
    }
    return i;
}
int AudioCapThread::releaseAudioSource(int hdl)
{
    if(hdl+1>=MAX_AUDIO_DATA_CB_NUM)
        return false;

    if(AudioSrc[hdl].isUsed==1)
    {
        AudioSrc[hdl].isUsed=0;
        AudioSrc[hdl].mDataCb=0;
        AudioSrc[hdl].mUsrDat=0;
        AudioSrc[hdl].cbStat=AUDIO_SINK_STAT_CLOSE;
        mNewCbCnt--;
    }
    return 0;

}


bool AudioCapThread::AutAudioCapThread()
{
    unsigned int card = 0;
    unsigned int device = 0;
    unsigned int channels = 2;
    unsigned int rate = 44100;
    unsigned int bits = 16;
    unsigned int period_size = 256;
    unsigned int period_count = 4;
    enum pcm_format format;

    format = PCM_FORMAT_S16_LE;

    //format = PCM_FORMAT_S32_LE;
    config.channels = channels;
    config.rate = rate;
    config.period_size = period_size;
    config.period_count = period_count;
    config.format = format;
    config.start_threshold = 0;
    config.stop_threshold = 0;
    config.silence_threshold = 0;


    //null state
    if(mAudioCapThreadState==AUDIO_CAP_STATE_NULL)
    {
#if 0
        struct mixer *mixer = mixer_open(0);
        if (!mixer)
        {
            fprintf(stderr, "Failed to open mixer\n");
            return 0;
        }

        tinymix_set_value(mixer, 23, "1");
        tinymix_set_value(mixer, 30, "1");
        mixer_close(mixer);
#else
        //system("tinymix  30 1");
        //system("tinymix  23 1");
#endif
        pcm = pcm_open(card, device, PCM_IN, &config);
        if (!pcm || !pcm_is_ready(pcm))
        {
            fprintf(stderr, "Unable to open PCM device (%s)\n",
                    pcm_get_error(pcm));
            return true;
        }
        buf_size = pcm_frames_to_bytes(pcm, pcm_get_buffer_size(pcm));
        buffer = (char*)malloc(buf_size);

        if (!buffer)
        {
            fprintf(stderr, "Unable to allocate %d bytes\n", buf_size);
            free(buffer);
            pcm_close(pcm);
            return true;
        }
        buffer_zero= (char*)malloc(buf_size);
        if (!buffer_zero)
        {
            ALOGW("  buffer_zero fail size =%d==-----------",buf_size);
        }
        else
        {
            memset(buffer_zero,0,buf_size);
        }
        ALOGW("  mAudioCapThreadState==AUDIO_CAP_STATE_INIT-----------");
        mAudioCapThreadState=AUDIO_CAP_STATE_INIT;
        pthread_mutex_lock(&mAudioCapMutex);
        pthread_cond_wait(&mAudioCapCond, &mAudioCapMutex);
        pthread_mutex_unlock(&mAudioCapMutex);


        ALOGW("  mAudioCapThreadState==AUDIO_CAP_STATE_INIT----change =%d-------",mAudioCapThreadState);
        //mAudioCapThreadState=AUDIO_CAP_STATE_INIT;

        return true;

    }

    if(mAudioCapThreadState==AUDIO_CAP_STATE_STARTED)
    {
        //start state
        //ALOGE("mAudioCapThreadState==AUDIO_CAP_STATE_STARTED-------%p-ize=%d---",buffer,buf_size);
        if(!pcm_read(pcm, buffer, buf_size))
        {
            //ALOGV("pcm_read ok---%p----%d---%x %x %x-",buffer,buf_size,buffer[0],buffer[10],buffer[20]);
            int i;
            for(i=0; i<MAX_AUDIO_DATA_CB_NUM; i++)
            {
                if((AudioSrc[i].isUsed==1)&&(AudioSrc[i].cbStat==AUDIO_SINK_STAT_OPEN)&&(AudioSrc[i].mDataCb!=NULL))
                {
                    //typedef void (*audiodata_callback)(int32_t msgType,nsecs_t timestamp,int card,int device,
                    //            char *dataPtr,int dsize, void* user);

                    int64_t curtime=(int64_t)systemTime();
                    int64_t nowpts=curtime;//-mCurFrameTimestamp;
                    mCurFrameTimestamp = (int64_t)systemTime();
                    AudioSrc[i].mDataCb(1,nowpts,card,device,
                                        buffer,buf_size,
                                        (void* )AudioSrc[i].mUsrDat);
                }
                if((AudioSrc[i].isUsed==1)&&(AudioSrc[i].cbStat==AUDIO_SINK_STAT_OPEN_MUTE)&&(AudioSrc[i].mDataCb!=NULL))
                {
                    //typedef void (*audiodata_callback)(int32_t msgType,nsecs_t timestamp,int card,int device,
                    //            char *dataPtr,int dsize, void* user);

                    int64_t curtime=(int64_t)systemTime();
                    int64_t nowpts=curtime;//-mCurFrameTimestamp;
                    mCurFrameTimestamp = (int64_t)systemTime();
                    AudioSrc[i].mDataCb(1,nowpts,card,device,
                                        buffer_zero,buf_size,
                                        (void* )AudioSrc[i].mUsrDat);
                }
            }
#if 0
            if(cap_cnt>0)
            {
                if(cap_cnt<1025)
                {
                    if (fwrite(buffer, 1, buf_size, file) != buf_size)
                    {
                        fprintf(stderr,"Error capturing sample\n");
                        //break;
                    }
                    else
                    {

                        cap_cnt++;

                    }
                }
                else
                {
                    fclose(file);
                    cap_cnt=-1;
                }

            }
            if(cap_cnt==0)
            {
                init_wav();
                cap_cnt++;
            }
#endif
            bytes_read += buf_size;
        }
        else
        {
            ALOGE("read pcm fail---%d(%s)--------",errno,strerror(errno));
            usleep(22000);
            int i;
            for(i=0; i<MAX_AUDIO_DATA_CB_NUM; i++)
            {
                if((AudioSrc[i].isUsed==1)&&(AudioSrc[i].cbStat==AUDIO_SINK_STAT_OPEN_MUTE)&&(AudioSrc[i].mDataCb!=NULL))
                {
                    int64_t curtime=(int64_t)systemTime();
                    int64_t nowpts=curtime;//-mCurFrameTimestamp;
                    mCurFrameTimestamp = (int64_t)systemTime();
                    AudioSrc[i].mDataCb(1,nowpts,card,device,
                                        buffer_zero,buf_size,
                                        (void* )AudioSrc[i].mUsrDat);
                }
            }
        }
    }
    //stop state

    if(mAudioCapThreadState==AUDIO_CAP_STATE_PAUSED)
    {

        ALOGE("  mAudioCapThreadState==AUDIO_CAP_STATE_PAUSED-----------");
        pthread_mutex_lock(&mAudioCapMutex);
        pthread_cond_wait(&mAudioCapCond, &mAudioCapMutex);
        pthread_mutex_unlock(&mAudioCapMutex);
        return true;
    }

    if(mAudioCapThreadState==AUDIO_CAP_STATE_EXIT)
    {
        ALOGE("  mAudioCapThreadState==AUDIO_CAP_STATE_EXIT-----------");
        if(buffer)
        {
            free(buffer);
            buffer=NULL;
        }
        if(buffer_zero)
        {
            free(buffer_zero);
            buffer_zero=NULL;
        }

        if(pcm)
        {
            pcm_close(pcm);
            pcm=NULL;
        }
    }
    return true;

}

