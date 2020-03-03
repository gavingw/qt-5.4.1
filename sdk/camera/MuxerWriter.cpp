/*
 * Copyright (c) 2008-2016 Allwinner Technology Co. Ltd.
 * All rights reserved.
 *
 * File : MuxerWriter.c
 * Description : MuxerWriter
 * History :
 *
 */

#include "MuxerWriter.h"
#include <sdklog.h>
#include "CameraFileCfg.h"
#include <sys/time.h>

typedef struct fdmgr_fds
{
    int fd;
    int cnt;
} fdmgr_fds_t;
#define MAX_MGR_FDCNT 8
#define WRITE_CNT_FOR_SYNC 4

fdmgr_fds_t muxfdmgr_fds[MAX_MGR_FDCNT];

static long long GetNowUs()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000000 + now.tv_usec;
}

int __CdxRead(CdxWriterT *writer, void *buf, int size)
{
    MuxerWriterT *muxer_writer = (MuxerWriterT*)writer;
    unsigned char *pbuf = (unsigned char *)buf;
    int ret = 0;
    if (pbuf == NULL)
    {
        loge("buf is NULL\n");
        return -1;
    }
    if (muxer_writer->file_mode == FD_FILE_MODE)
    {
        ret = read(muxer_writer->fd_file, pbuf, size);
        char nn[256];
        sprintf(nn,"/proc/self/fd/%d",muxer_writer->fd_file);
        char namer[256];
        memset(namer,0,256);
        readlink(nn, namer, 256);
        ALOGW("__CdxRead name %s fd=%d pbuf=%p len=%d",namer,muxer_writer->fd_file, pbuf, size);
        if (ret < 0)
        {
            loge("CdxRead read failed\n");
            return -1;
        }
    }
    else if (muxer_writer->file_mode == FP_FILE_MODE)
    {
        ret = fread(pbuf, 1, size, muxer_writer->fp_file);
        if (ret < 0)
        {
            loge("CdxRead fread failed\n");
            return -1;
        }
    }
    return ret;
}


void* (file_sync)(void*)
{
	sync();
}


int __CdxWrite(CdxWriterT *writer, void *buf, int size)
{
    MuxerWriterT *muxer_writer = (MuxerWriterT*)writer;
    unsigned char *pbuf = (unsigned char *)buf;
    int ret = 0;
    if (pbuf == NULL)
    {
        loge("buf is NULL\n");
        return -1;
    }
    if (muxer_writer->file_mode == FD_FILE_MODE)
    {
        char nn[256];
        sprintf(nn,"/proc/self/fd/%d",muxer_writer->fd_file);
        char namer[256];
        memset(namer,0,256);
        readlink(nn, namer, 256);

        //ALOGW("write name %s fd=%d pbuf=%p len=%d",namer,muxer_writer->fd_file, pbuf, size);
        int camid;
        if(strstr(muxer_writer->file_path,"rear")!=NULL)
        {
            camid=1;
        }
        else
        {
            camid=0;
        }
        ret = write(muxer_writer->fd_file, pbuf, size);
        if (ret < 0)
        {
            loge("CdxWrite write failed %s fd=%d name=%s\n",strerror(errno),muxer_writer->fd_file,muxer_writer->file_path);
            return -1;
        }
        muxfdmgr_fds[camid].cnt++;
        if(muxfdmgr_fds[camid].cnt%WRITE_CNT_FOR_SYNC==0)//sync every writecnt
        {
            long long timestart= GetNowUs();

			pthread_t id;
			struct timespec ts;
			int s;

			ret = pthread_create(&id, NULL, file_sync, NULL); 
			if(0 != ret){
				printf("Create pthread error!\n");
				return -1;
			}
			
			if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
	            /* Handle error */
				printf("clock_gettime fild\n",s);
	        }
			//printf("tv_sec=%uld,tv_nsec=%uld\n",ts.tv_sec,ts.tv_nsec);
			ts.tv_sec += 4;

			s = pthread_timedjoin_np(id, NULL, &ts);
			if (s != 0) {
			   switch (s)
			   {
				case ETIMEDOUT :    //110
					printf("file_sync time out s=%d\n",s);
					ret = -1;
					break;
				
				default:
					break;
			   }
			   printf("file_sync errno=%d\n",s);
			}
			

			/*
            ret = fsync(muxer_writer->fd_file);//fixbug :for remove card ,lost 10s video
            if (ret < 0)
            {
                ALOGE("CdxWrite fsync failed %s fd=%d camid=%d\n",strerror(errno),muxer_writer->fd_file,camid);
            }
			*/
			
            long long timeend= GetNowUs();

            ALOGD("camid %d fsync time is %lld ",camid,timeend-timestart);
        }
    }
    else if (muxer_writer->file_mode == FP_FILE_MODE)
    {
        ret = fwrite(pbuf, 1, size, muxer_writer->fp_file);
        if (ret < 0)
        {
            loge("CdxWrite fwrite failed\n");
            return -1;
        }
    }
    return ret;
}

long __CdxSeek(CdxWriterT *writer, long moffset, int mwhere)
{
    MuxerWriterT *muxer_writer = (MuxerWriterT*)writer;
    long ret = 0;
    if (muxer_writer->file_mode == FD_FILE_MODE)
    {
        char nn[256];
        sprintf(nn,"/proc/self/fd/%d",muxer_writer->fd_file);
        char namer[256];
        memset(namer,0,256);
        readlink(nn, namer, 256);
        ALOGW("seek name %s fd=%d where =%d off =%ld",namer,muxer_writer->fd_file,  mwhere,moffset);
        ret = lseek(muxer_writer->fd_file, moffset, mwhere);
        if (ret < 0)
        {
            loge("CdxSeek lseek failed\n");
            return -1;
        }
    }
    else if(muxer_writer->file_mode == FP_FILE_MODE)
    {
        ret = fseek(muxer_writer->fp_file, moffset, mwhere);
        if (ret < 0)
        {
            loge("CdxSeek fseek failed\n");
            return -1;
        }
    }
    return ret;
}

long __CdxTell(CdxWriterT *writer)
{
    MuxerWriterT *muxer_writer = (MuxerWriterT*)writer;
    if (muxer_writer->file_mode == FD_FILE_MODE)
    {
        char nn[256];
        sprintf(nn,"/proc/self/fd/%d",muxer_writer->fd_file);
        char namer[256];
        memset(namer,0,256);
        readlink(nn, namer, 256);

        ALOGW("write tell %s fd=%d ",namer,muxer_writer->fd_file);
#ifdef CDX_V27
        return CdxWriterSeek(writer, 0, SEEK_CUR);
#else
        return writer->cdxSeek(writer, 0, SEEK_CUR);
#endif

    }
    else if (muxer_writer->file_mode == FP_FILE_MODE)
    {
        return ftell(muxer_writer->fp_file);
    }
    return 0;
}
//#define OPEN_TEST
#define FALLOC_FL_KEEP_SIZE 0x01 /* default is extend size */

int xCreateMyFile(char * szFileName,int nFileLength)
{
    int res,fd;
    //ALOGW("CreateMyFile %s",szFileName);
    fd = open(szFileName, O_RDWR | O_CREAT, 0666);

    if (fd < 0)
    {
        fprintf(stderr,"openfail line %d %s\n",__LINE__, strerror(errno));
        close(fd);
        return -1;
    }
    //ALOGW("fallocate start!len=%d\n",nFileLength);
    if (res = fallocate(fd, FALLOC_FL_KEEP_SIZE, 0, nFileLength))
    {
        fprintf(stderr, "fallocate line %d len=%d %s\n",__LINE__, nFileLength,strerror(errno));
        close(fd);
		sync();
        return -2;
    }

    close(fd);
	sync();
    return 0;
}

int MWOpen(CdxWriterT *writer)
{
    MuxerWriterT *muxer_writer = (MuxerWriterT*)writer;
    if (muxer_writer->file_path == NULL)
    {
        loge("muxer_writer->file_path is NULL\n");
        return -1;
    }
    if (muxer_writer->file_mode == FD_FILE_MODE)
    {
        ALOGW("MWOpen--%s-",muxer_writer->file_path);
        int camid;
        if(strstr(muxer_writer->file_path,"rear")!=NULL)
        {
            camid=1;
        }
        else
        {
            camid=0;
        }
        muxer_writer->fd_file = open(muxer_writer->file_path, O_RDWR | O_CREAT, 0666);

        if (muxer_writer->fd_file < 0)
        {
            loge("muxer_writer->fd_file open failed %s\n",muxer_writer->file_path);
            return -1;
        }
        muxfdmgr_fds[camid].fd=muxer_writer->fd_file;
        ALOGW("MWOpen--%s-fd=%d----camid=%d",muxer_writer->file_path,muxer_writer->fd_file,camid);
        muxfdmgr_fds[camid].cnt=0;
    }
    else if (muxer_writer->file_mode == FP_FILE_MODE)
    {
        muxer_writer->fp_file = fopen(muxer_writer->file_path, "wb+");
        if (muxer_writer->fp_file == NULL)
        {
            loge("muxer_writer->fp_file fopen failed\n");
            return -1;
        }
    }
    return 0;
}

int MWClose(CdxWriterT *writer)
{
    MuxerWriterT *muxer_writer = (MuxerWriterT*)writer;
    int ret = 0;
    if (muxer_writer->file_mode == FD_FILE_MODE)
    {
        ALOGW("close---fd=%d path=%s camfd0=%d camfd1=%d",muxer_writer->fd_file,muxer_writer->file_path,muxfdmgr_fds[0].fd,muxfdmgr_fds[1].fd);
        char nn[256];
        sprintf(nn,"/proc/self/fd/%d",muxer_writer->fd_file);
        char namer[256];
        memset(namer,0,256);
        readlink(nn, namer, 256);
        ALOGW("close---%d %s",muxer_writer->fd_file,namer);
        //fsync(muxer_writer->fd_file);//too slow
        ret = close(muxer_writer->fd_file);
        ALOGW("close---%d %s",muxer_writer->fd_file,namer,ret);
        muxer_writer->fd_file=-1;
    }
    else if (muxer_writer->file_mode == FP_FILE_MODE)
    {
        logd("fclose---");
        fflush(muxer_writer->fp_file);
        //int fd=fileno(muxer_writer->fp_file);//too slow
        //fsync(fd);
        ret = fclose(muxer_writer->fp_file);
    }
    if (ret < 0)
    {
        loge("__CdxClose() failed %s",strerror(errno));
    }
    return ret;
}

static int __CdxClose(CdxWriterT *writer)
{
    return 0;
}

static struct CdxWriterOps writerOps =
{

};

CdxWriterT *CdxWriterCreat()
{
    MuxerWriterT *muxer_writer = NULL;

    if ((muxer_writer = (MuxerWriterT*)malloc(sizeof(MuxerWriterT))) == NULL)
    {
        loge("CdxWriter creat failed\n");
        return NULL;
    }
    memset(muxer_writer, 0, sizeof(MuxerWriterT));
    if ((muxer_writer->file_path = (char *)malloc(128)) == NULL)
    {
        loge("MyWriter->file_path malloc failed");
        free(muxer_writer);
        return NULL;
    }
    memset(muxer_writer->file_path, 0, 128);
    writerOps.read   = __CdxRead;
    writerOps.write  = __CdxWrite;
    writerOps.seek   = __CdxSeek;
    writerOps.tell   = __CdxTell;
    writerOps.close  = __CdxClose;
    muxer_writer->cdx_writer.ops = &writerOps;

    return &muxer_writer->cdx_writer;
}

void CdxWriterDestroy(CdxWriterT *writer)
{
    MuxerWriterT *muxer_writer = (MuxerWriterT*)writer;
    if (muxer_writer == NULL)
    {
        logw("writer is NULL, no need to be destroyed\n");
        return;
    }
    if (muxer_writer->file_path)
    {
        free(muxer_writer->file_path);
        muxer_writer->file_path = NULL;
    }
    free(muxer_writer);
}
