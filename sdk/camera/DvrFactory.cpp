/*
 * \file        cameratester.c
 * \brief
 *
 * \version     1.0.0
 * \date        2012~{e9~}?6?~{2~}6??
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#include "videodev2_new.h"

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/fb.h>
#include <pthread.h>
#include <sys/syscall.h>

#include "aw_ini_parser.h"

#include "CameraDebug.h"

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vencoder.h"
#include "CdxMuxer.h"
#include <time.h>

#include "Rtc.h"
#include "StorageManager.h"
#include "DvrFactory.h"
#include "CameraFileCfg.h"
#include "rtspapi.h"

#include "posixTimer.h"
#include "DvrRecordManager.h"

using namespace android;

#include <sdklog.h>

#include "CameraDebug.h"



#include "V4L2CameraDevice2.h"
#include "PreviewWindow.h"
#include "CameraHardware2.h"
#include "HALCameraFactory.h"
#include "CameraHardwareInterface.h"
#ifdef REC_ENCODE
#include "awrecorder.h"

#endif

#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#ifdef USE_CDX_LOWLEVEL_API
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vencoder.h"
#include "aencoder.h"

#include "CdxMuxer.h"
#include <time.h>
#endif
#include "Rtc.h"
#include "StorageManager.h"
#include "DvrFactory.h"
#include "CameraFileCfg.h"
#include "hwdisp2.h"

#define FRANT_VIDEO   "frontVideo"
#define REAR_VIDEO    "rearVideo"
#define LOCK_VIDEO    "lockVideo"
#define PARK_MONITOR  "parkMonitor"


#define _ENCODER_TIME_
#ifdef _ENCODER_TIME_
static long long GetNowUs()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000000 + now.tv_usec;
}

long long time1=0;
long long time2=0;
long long time3=0;
#endif
#define gettid() syscall(__NR_gettid)

char path_picture[2][128] = {"/mnt/sdcard/mmcblk1p1/frontPicture","/mnt/sdcard/mmcblk1p1/rearPicture"};
unsigned long max_pic_num = 512;
int cur_pic_idx[2][1]={0,0};
char pic_names[2][CM_MAX_PICTURE_NUM][128]={};
int dvr_factory::generatepicFile()
{
	
    int fd;
    int i;
    struct tm *tm=NULL;
    
    #ifdef SMART_RECORD
	    Mutex::Autolock locker(&gfileLock);
	    file_status_t *p =&gFilemanger;
    #else
		file_status_t *p =&mFilemanger;	
	#endif
	
    char buf[128]= {0};
    buf[128]= 0;
    char *suffix[]={"front", "rear"};
    int  idx = 0;
    idx = config_get_normalpicidx(mCameraId);
    int l32_CameraId = (360 == mCameraId)?  0: mCameraId;
    if (idx > max_pic_num)
    {
        ALOGI("pic file idx %d lager than max_idx %d)\n", idx, max_pic_num);
        idx = 0;
    }
	
    config_set_normalpicidx(mCameraId, idx + 1);
	//remove the old idx file
    if(strlen(pic_names[l32_CameraId][idx]))
    {
        remove(pic_names[l32_CameraId][idx]);
        ALOGI("rm pic file (%s cur_file_idx %d)\n", pic_names[l32_CameraId][idx],idx);
    }
    ALOGV("%s %d", __FUNCTION__, __LINE__);
    time_t now = getDateTime(&tm);

    snprintf(buf, 127,"%s%s%d-%04d%02d%02d_%02d%02d%02d_%s%s",path_picture[l32_CameraId],"/",idx,tm->tm_year + 1900, tm->tm_mon + 1,
             tm->tm_mday, tm->tm_hour,tm->tm_min, tm->tm_sec, suffix[l32_CameraId], EXT_PIC );

	fd = open(buf, O_RDWR | O_CREAT, 0666);
    if (fd < 0)
    {
        ALOGE("failed to open file '%s'!!\n", buf);
        return RET_IO_OPEN_FILE;
    }
	
    //save the new idx name
    sprintf(pic_names[l32_CameraId][idx],"%s",buf);
    ALOGW("PictureName:%s\n",buf);
    return fd;
}
int dvr_factory::generateThumbFile()
{
	
    int fd;
    struct tm *tm=NULL;
    
    #ifdef SMART_RECORD
		Mutex::Autolock locker(&gfileLock); 
		file_status_t *p =&gFilemanger;
   #else
		file_status_t *p =&mFilemanger;	
	#endif
	
    char buf[128]= {0};
    buf[128]= 0;
    char * suffix[] = {"front", "rear"};
    int l32_CameraId = (360 == mCameraId)?  0: mCameraId;
    
    ALOGV("%s %d", __FUNCTION__, __LINE__);
	#ifdef SMART_RECORD
    sprintf(buf, "%s%s%s%d%c%s%s%s",p->cur_filedir[l32_CameraId],"/",CM_THUMB_DIR,p->cur_file_idx,'-',"thumb_",suffix[l32_CameraId],EXT_PIC);
	#else
    sprintf(buf, "%s%s%s%d%c%s%s%s",p->cur_filedir[0],"/",CM_THUMB_DIR,p->cur_file_idx,'-',"thumb_",suffix[l32_CameraId],EXT_PIC);
	#endif
	//ALOGW("generateThumbFile %s",buf);
    fd = open(buf, O_RDWR | O_CREAT, 0666);
    if (fd < 0)
    {
        ALOGE("failed to open file '%s'!!\n", buf);
        return RET_IO_OPEN_FILE;
    }
    return fd;
}

int dvr_factory::savePicture(void* data, int size, int fd)
{
    int bytes_write = 0;
    int bytes_to_write = size;
    void *ptr = data;

    ALOGW("savePciture, size is %d\n", size);

    while((bytes_write = write(fd, ptr, bytes_to_write)) )
    {
        if((bytes_write == -1) && (errno != EINTR))
        {
            ALOGE("wirte error: %s\n", strerror(errno));
            break;
        }
        else if(bytes_write == bytes_to_write)
        {
            break;
        }
        else if( bytes_write > 0 )
        {
            ptr = (void *)((char *)ptr + bytes_write);
            bytes_to_write -= bytes_write;
        }
    }
    if(bytes_write == -1)
    {
        ALOGE("save picture failed: %s\n", strerror(errno));
        return -1;
    }

    fsync(fd);
    close(fd);

    return 0;
}

int dvr_factory::setDuration(int sec)
{
    if(sec==0)
        return -1;
    Mutex::Autolock locker(&mObjectLock);
	ALOGV("setDuration %d",sec);
    mDuration=sec;
    return 0;

}
int dvr_factory::getDuration()
{

    return mDuration;

}

/********************scan picture path  start*******************************/
int scan_picture(const char *path, int mCameraId)
{
    DIR    *dir_ptr;
    struct dirent  *direntp;
    char rmbuf[256] = {};
    char tmpbuf[256] = {};
    int l32_CameraId = (360 == mCameraId)?  0: mCameraId;
    
    if ( ( dir_ptr = opendir( path) ) == NULL )
    {
        ALOGE("open picture path %s failed\n", path);
        return -1;
    }
    else
    {
        int i = 0;
        int idx = 0;
        //chdir(path);
        while ( ( direntp = readdir( dir_ptr ) ) != NULL )
        {
			ALOGV("dbg scan_picture d_name=%s",direntp->d_name);
            for(i=0; i<strlen(direntp->d_name); ++i)
            {
                if(direntp->d_name[i]=='-')
                {
                	if(strstr(direntp->d_name,"mp4"))
                	{
                		ALOGV("dbg scan_picture this file is mp4,continue!");
                		continue;
                	}

                    memset(tmpbuf,0,256);
                    memcpy(tmpbuf,direntp->d_name,i);
                    idx=atoi(tmpbuf);
                    if((idx < CM_MAX_PICTURE_NUM)&&(idx>=0) &&(idx <= max_pic_num))
                    {
                        if(strlen(pic_names[l32_CameraId][idx])==0)
                        {
                            sprintf(pic_names[l32_CameraId][idx],"%s/%s",path, direntp->d_name);
                        }
                        else
                        {
                            memset(rmbuf,0,256);
                            strcat(rmbuf,direntp->d_name);

                            if(strstr(pic_names[l32_CameraId][idx],rmbuf)!=NULL)
                            {
                                int rmret = 0;
                                ALOGI("find duplicate pic  so rm old %s idx=%d  name=%s \n",pic_names[l32_CameraId][idx],idx,direntp->d_name);
                                rmret = remove(pic_names[l32_CameraId][idx]);
                                ALOGI("rmpicfile ret %s",rmret==0?"OK":"FAIL");
                                sprintf(pic_names[l32_CameraId][idx],"%s/%s",path, direntp->d_name);
                            }
                        }
                        //LOGI("parse success  idx= %d  name= %s \n",idx,direntp->d_name);
                    }
                    else
                    {
                        char tname[256] = {};
                        sprintf(tname,"%s/%s",path_picture[l32_CameraId]);
                        remove(tname); 
                    }
                }
            }

        }
        closedir(dir_ptr);
    }
        return 0;

/********************scan picture path end*******************************/
}

bool dvr_factory::initFileListDir( char * dirname )

{
    DIR    *dir_ptr;

    struct dirent  *direntp;
    int i;
    int idx;
    char tmpbuf[256];
    char rmbuf[256];
	
	char real_path[256];
	char camera_path[256];
	char thumb_path[256];
	
	#ifdef SMART_RECORD
		Mutex::Autolock locker(&gfileLock);
	    file_status_t *p =&gFilemanger;
	#else
		file_status_t *p =&mFilemanger;	
	#endif
	
    unsigned long long  totMB=0;
    int rmret=0;
    int l32_CameraId = (360 == mCameraId)?  0: mCameraId;
    /*
      opendir函数用来打开参数name指定的目录，
      并返回DIR *形态的目录流
    */

    if(l32_CameraId > 2)
    {
		l32_CameraId = 0;
    }
    
    F_LOG;
    if(dirname==NULL)
    {
        ALOGE("dirname ==null");
        return false;
    }

    int len = strlen(dirname);

    if(len<2)
    {
        ALOGE("video dir can toshort ");
        return false;
    }

	strcpy(tmpbuf,dirname);
    for(i=0; i<len; i++)
    {

        if(tmpbuf[len-1-i]=='/')
        {
            tmpbuf[len-1-i]=0;
        }
        else
        {
            break;

        }
    }

    rmret = readlink(tmpbuf,real_path,sizeof(real_path));
    if(rmret<0)
    {
        ALOGV("mount path not a symlink %s err=%d",tmpbuf,errno);

        strcpy(real_path,tmpbuf);
    }
    else
    {
        ALOGV("mount real path is %s \r\n",real_path);
    }

    if(isMounted(real_path))
    {
        ALOGV("-------sdmmc------mounted");
		storage_state=1;

    }
    else
    {
        ALOGE("!!!-------sdmmc------unmounted");
        return false;
    }
//create normal video path
    const char *path_video[] = {
        FRANT_VIDEO,
        REAR_VIDEO,
        FRANT_VIDEO,
        REAR_VIDEO,
    };
	
    memset(camera_path, 0, sizeof(camera_path));
	sprintf(camera_path,"%s/%s",real_path,path_video[l32_CameraId]);	
    strcpy(tmpbuf,camera_path);
    if(createdir(tmpbuf)!=0)
    {
        ALOGE("create camera_path dir %s fail",tmpbuf);
        return false;
    }
    memset(thumb_path, 0, sizeof(thumb_path));
	sprintf(thumb_path,"%s/%s",camera_path,CM_THUMB_DIR);
    strcpy(tmpbuf,thumb_path);
    if(createdir(tmpbuf)!=0)
    {
        ALOGE("create thumb dir %s fail",tmpbuf);
        //return false;
    }
//create normal picture path
char *path_pic[] = {
	"frontPicture",
	"rearPicture",
	};

    memset(path_picture[l32_CameraId], 0, sizeof(path_picture[l32_CameraId]));
	sprintf(path_picture[l32_CameraId],"%s/%s",real_path,path_pic[l32_CameraId]);	
    strcpy(tmpbuf,path_picture[l32_CameraId]);
    if(createdir(tmpbuf)!=0)
    {
        ALOGE("create picture_path dir %s fail",tmpbuf);
        return false;
    }
//create lockvideo path
    memset(rmbuf, 0, sizeof(rmbuf));
	sprintf(rmbuf,"%s/%s",real_path,LOCK_VIDEO);
    strcpy(tmpbuf,rmbuf);
    if(createdir(tmpbuf)!=0)
    {
        ALOGE("create lock dir %s fail",tmpbuf);
        //return false;
    }
    memset(thumb_path, 0, sizeof(thumb_path));
	sprintf(thumb_path,"%s/%s",tmpbuf,CM_THUMB_DIR);
    strcpy(tmpbuf,thumb_path);
    if(createdir(tmpbuf)!=0)
    {
        ALOGE("create lockVideo thumb dir %s fail",tmpbuf);
        //return false;
    }

//create parkmonitor path

    memset(rmbuf, 0, sizeof(rmbuf));
	sprintf(rmbuf,"%s/%s",real_path,PARK_MONITOR);
    strcpy(tmpbuf,rmbuf);
    if(createdir(tmpbuf)!=0)
    {
        ALOGE("create parkMonitor dir %s fail",tmpbuf);
        //return false;
    }
    memset(thumb_path,0, sizeof(thumb_path));
	sprintf(thumb_path,"%s/%s",tmpbuf,CM_THUMB_DIR);
    strcpy(tmpbuf,thumb_path);
    if(createdir(tmpbuf)!=0)
    {
        ALOGE("create parkMonitor thumb dir %s fail",tmpbuf);
        //return false;
    }

	#ifdef SMART_RECORD
    memset(p->cur_filedir[l32_CameraId],0,CM_MAX_FILE_LEN);
    strcpy(p->cur_filedir[l32_CameraId],camera_path);
	#else
	memset(p->cur_filedir[0],0,CM_MAX_FILE_LEN);
    strcpy(p->cur_filedir[0],camera_path);
    for(i=0; i<CM_MAX_RECORDFILE_NUM; i++){
        memset(p->cur_filename[i],0,CM_MAX_FILE_LEN);
    }
	#endif

    if ( ( dir_ptr = opendir( camera_path ) ) == NULL )
    {
    }
    else
    {
        //totMB简单算一下卡的容量，需要换成free+mp4-lock-save;才准确
        totMB = totalSize(real_path);
        unsigned long afile_size = ((mDuration*encode_param.bit_rate)>>23)+(13*mDuration)/60;

        ALOGW("----total size=%llu,afilesize =%lu syctime=%d bit=0x%x",totMB,afile_size,mDuration,encode_param.bit_rate);
		if(afile_size==0){
        	ALOGW("get afilesize =0");
            afile_size=(totMB-RESERVED_SIZE)/CM_MAX_RECORDFILE_NUM;
        }
        
		#ifdef SMART_RECORD
        //= (bit_rate*60/8)+13 => bit_rate*7.5+13 ==>bit_rate*8+13 ==>(bit_rate>>20)<<8 + 13==>(bit_rate>>17)+ 13;
        if(gAFileOccupySizePerMinMb < ((encode_param.bit_rate>>17)+ 23)){
			gAFileOccupySizePerMinMb = (encode_param.bit_rate>>17)+ 23;  	
		}else{
			ALOGV("dbg-rec only use the higher bit_rate[%d] to calc A file occupy size",(gAFileOccupySizePerMinMb-23)>>3);
		}
		
		p->cur_max_filenum = (totMB-RESERVED_SIZE )/((mDuration/60)*gAFileOccupySizePerMinMb);
		
		#else
		p->cur_max_filenum = (totMB-RESERVED_SIZE )/( 2 * afile_size);
		#endif
		
        if(p->cur_max_filenum>CM_MAX_RECORDFILE_NUM)
            p->cur_max_filenum=CM_MAX_RECORDFILE_NUM;
        config_set_normalvideomax(mCameraId, p->cur_max_filenum);
        //config_set_lockvideomax(mCameraId, p->cur_max_filenum);
        ALOGV("dbg-rec max_filenum=%d",p->cur_max_filenum);
        //  150k * 6 ~ 1M 
        max_pic_num = (totMB-RESERVED_SIZE * 2);
        config_set_normalpicmax(mCameraId, max_pic_num);
        //ALOGW("----max file num =%d ",p->cur_max_filenum);
        //LOGI("--------dvr mode%d------%d,%d,%d,%d",p->camera_mode,totMB,afile_size,p->cur_max_filenum,p->pcamera_param->continue_record);
        //chdir(camera_path);
        /*
              readdir返回参数dir目录流的下个目录进入点
            */
        while ( ( direntp = readdir( dir_ptr ) ) != NULL )
        {
            if(strstr(direntp->d_name,CM_SAVE_FILE_MARK)!=NULL)
                continue;
            if(strstr(direntp->d_name,CM_LOCK_FILE_MARK)!=NULL)
                continue;

            for(i=0; i<strlen(direntp->d_name); i++)
            {
                if(direntp->d_name[i]=='-')
                {
                    memset(tmpbuf,0,256);
                    memcpy(tmpbuf,direntp->d_name,i);
                    idx=atoi(tmpbuf);
                    if((idx < CM_MAX_RECORDFILE_NUM)&&(idx>=0))
                    {
                        if(strlen(p->cur_filename[idx])==0)
                        {
                            strcpy(p->cur_filename[idx],camera_path);
                            strcat(p->cur_filename[idx],"/");
                            strcat(p->cur_filename[idx],direntp->d_name);
                            //ALOGI("cur file %d %s ",idx,p->cur_filename[idx]);
                            p->cur_dir_file_num++;
                        }
                        else
                        {
                            memset(rmbuf,0,256);
                            strcat(rmbuf,direntp->d_name);

                            if(strstr(p->cur_filename[idx],rmbuf)==NULL)
                            {
                                ALOGW("find duplicate  so rm old %s idx=%d  name=%s \n",p->cur_filename[idx],idx,direntp->d_name);
                                rmret = remove(p->cur_filename[idx]);
                                ALOGW("rmfile ret %s",rmret==0?"OK":"FAIL");
                                strcpy(p->cur_filename[idx],camera_path);
                                strcat(p->cur_filename[idx],"/");
                                strcat(p->cur_filename[idx],direntp->d_name);
                                ALOGW("newidx file %s len=%d",p->cur_filename[idx],strlen(p->cur_filename[idx]));
								
								//remove thumb
								#ifdef SMART_RECORD
								sprintf(tmpbuf, "%s%s%s%d%c%s%s",p->cur_filedir[l32_CameraId],"/",CM_THUMB_DIR,idx,'-',"thumb",EXT_PIC);
								#else
								sprintf(tmpbuf, "%s%s%s%d%c%s%s",p->cur_filedir[0],"/",CM_THUMB_DIR,idx,'-',"thumb",EXT_PIC);
								#endif
								remove(tmpbuf);
                            }

                        }
                        //LOGI("parse success  idx= %d  name= %s \n",idx,direntp->d_name);
                    }
                    else
                    {
                        //printf("can't parse idx= %d  name= %s \n",idx,direntp->d_name);
                    }
                }
            }

        }
		
        scan_picture(path_picture[l32_CameraId], mCameraId);
        closedir(dir_ptr);
		sync();
    }
    return true;
}
int dvr_factory::allocFileSpace()
{
    if(needDeleteFiles())
    {

    }
    return 0;
}




#ifdef SMART_RECORD
#define FILE_NO_REMOVE 1
//if define then when card is full,rename old idx file or delete near  2files if space not enough
//if not define ,when card is full,remove old idx file,and re cale space for new file

#define FILE_HOLES_SYS_SUPPORT 1
//depend on FILE_NO_REMOVE,
//ifdefineFILE_NO_REMOVE and  FILE_HOLES_SYS_SUPPORT  use fat32 falloc patch ,see kernel fat pacth
//if not define only define FILE_NO_REMOVE when card is full,remove old idx file,and re cale space for new file

#define FALLOC_FL_KEEP_SIZE 0x01 /* default is extend size */

int xxCreateMyFile(char * szFileName,int nFileLength)
{
	int res,fd;  
	//ALOGW("CreateMyFile %s",szFileName);
   	fd = open(szFileName, O_RDWR | O_CREAT, 0666);
	
	if (fd < 0) {
		fprintf(stderr,"openfail line %d %s\n",__LINE__, strerror(errno));
		close(fd);
		return -1;
	}
	//ALOGW("fallocate start!len=%d\n",nFileLength);
	if (res = fallocate(fd, FALLOC_FL_KEEP_SIZE, 0, nFileLength)) {
		fprintf(stderr, "fallocate line %d len=%d %s\n",__LINE__, nFileLength,strerror(errno));
		fsync(fd);

		close(fd);
		return -2;
	}
	fsync(fd);
	close(fd); 
	return 0;
}

int dvr_factory::genfilename(char* name)
{
    char rec_filename[256];
    int rmidx=0,reidx=0;
    
	Mutex::Autolock locker(&gfileLock);
    file_status_t *p =&gFilemanger;
    char *suffix[] = {"front", "rear","front", "rear"};
	int l32_CameraId = (360 == mCameraId)?  0: mCameraId;
    if(name==NULL)
        return -1;
        
    //只使用0摄像头的配置保存索引
	p->cur_file_idx =config_get_camfileidx(0);    
	if(p->cur_file_idx >= p->cur_max_filenum)
    {
        p->cur_file_idx = 0;
    }

	unsigned long long as = availSize(p->cur_filedir[l32_CameraId]);
	unsigned long afile_size = ((mDuration*encode_param.bit_rate)>>23)+(13*mDuration)/60;
    int afile_blksize =( mDuration/60 ) * gAFileOccupySizePerMinMb;
	ALOGI("as is %lluM,re is %dM dur=%d bitrate=0x%x",as,(int)(afile_blksize + RESERVED_SIZE),mDuration,encode_param.bit_rate);

    rmidx = p->cur_file_idx;
    reidx=0;
    int ret;
    do
    {
        if(reidx >= CM_MAX_RECORDFILE_NUM)  //防止死循环
            break;
        else{
        	reidx++;
        }
        
        as = availSize(p->cur_filedir[l32_CameraId]);
        ALOGV("dbg-rec as is %lldM,re is %dM",as,(int)(afile_blksize+RESERVED_SIZE));
        	
        if(as >(afile_blksize+RESERVED_SIZE)){
            break;  //we have enough room
        }
        else
        {
            if(strlen(p->cur_filename[rmidx])!=0)
            {
            	ALOGV("dbg-rec no av room!rm [%d] %s,FileSize=%dM,OccupySizeMb=%d",rmidx,p->cur_filename[rmidx],
            					getFileSize(p->cur_filename[rmidx]),getFileOccupySizeMb(p->cur_filename[rmidx])  );
            	//remove thumb
				char buf2[CM_MAX_FILE_LEN];
		    	memset(buf2,0,CM_MAX_FILE_LEN);
				sprintf(buf2, "%s%s%s%d%c%s%s%s",p->cur_filedir[0],"/",CM_THUMB_DIR,p->cur_file_idx,'-',"thumb_",suffix[0],EXT_PIC);
				if(access(buf2, F_OK) == 0){
					remove(buf2);
				}
				
				memset(buf2,0,CM_MAX_FILE_LEN);
				sprintf(buf2, "%s%s%s%d%c%s%s%s",p->cur_filedir[1],"/",CM_THUMB_DIR,p->cur_file_idx,'-',"thumb_",suffix[1],EXT_PIC);
				if(access(buf2, F_OK) == 0){
					remove(buf2);
				}

				

				if(access(p->cur_filename[rmidx], F_OK) != 0){
					ALOGV("file[%d] %s doesn't exist,pass this file.",rmidx,p->cur_filename[rmidx]);
					memset(p->cur_filename[rmidx],0,CM_MAX_FILE_LEN);
					if(rmidx >= CM_MAX_RECORDFILE_NUM){
		                rmidx=0;
		            }else{
						rmidx++;
		            }
					continue;
				}
				int oldfileSize = getFileOccupySizeMb(p->cur_filename[rmidx]);
				if( afile_blksize  <= (oldfileSize - gAFileOccupySizePerMinMb) )
				{	//若旧文件空间比较大且大小是新的2倍以上，空间不匹配，直接删除
					ALOGV("dbg-rec file size dismatch,remove idx[%d] file now,oldfilesize=%d",rmidx,oldfileSize);
                	ret=remove(p->cur_filename[rmidx]);
					if(ret==0)
               			memset(p->cur_filename[rmidx],0,CM_MAX_FILE_LEN);
					else
						ALOGE("dbg-rec old file room is too large,but rm file %s fail %d",p->cur_filename[rmidx],errno);
				}
				else if(( afile_blksize <= oldfileSize ) && (afile_blksize  >= (oldfileSize - gAFileOccupySizePerMinMb)) )
				{	//若旧文件空间比需要创建空间大不到1倍，空间匹配，不删除
					if(rmidx == p->cur_file_idx){//can use curr file
						ALOGV("dbg-rec no need to rm[%d],oldfilesize=%d,just use the old one",rmidx,oldfileSize);
						break;
					}

					ALOGV("dbg-rec rmidx != curr_idx,can not use,rm[%d]",rmidx);
					
					ret=remove(p->cur_filename[rmidx]);
					if(ret==0){
						memset(p->cur_filename[rmidx],0,CM_MAX_FILE_LEN);
					}else{
						ALOGE("dbg-rec file room is match but rm file %s fail %d",p->cur_filename[rmidx],errno);
					}
					
				}else{														//若旧文件空间比需要的空间小
					ALOGV("dbg-rec need to rm[%d]",rmidx);
					ret=remove(p->cur_filename[rmidx]);
					if(ret==0){
						memset(p->cur_filename[rmidx],0,CM_MAX_FILE_LEN);
					}else{
						ALOGE("dbg-rec old file room is too small,but rm file %s fail %d",p->cur_filename[rmidx],errno);
					}

                }
            }

            if(rmidx >= CM_MAX_RECORDFILE_NUM)  
            {
                rmidx=0;
            }else{
				rmidx++;
            }
        }
    }
    while(1);
    struct tm *tm=NULL;
    time_t now = getDateTime(&tm);
    sprintf(rec_filename, "%s%s%d%c%04d%02d%02d_%02d%02d%02d_%s%s",p->cur_filedir[l32_CameraId],"/",p->cur_file_idx,'-',tm->tm_year + 1900, tm->tm_mon + 1,
            tm->tm_mday, tm->tm_hour,tm->tm_min, tm->tm_sec,suffix[l32_CameraId],EXT_VIDEO);

#ifdef FILE_NO_REMOVE
	#ifdef FILE_HOLES_SYS_SUPPORT

	int perfilesize=(gAFileOccupySizePerMinMb<<20)*(mDuration/60);
	if(access(p->cur_filename[p->cur_file_idx], F_OK) == 0)
	{
		ALOGW("dbg-rec t a file %s ",p->cur_filename[p->cur_file_idx]);
		ret = xxCreateMyFile(p->cur_filename[p->cur_file_idx],perfilesize);   //this is needed to prevent kernel crash
		if(ret < 0){
			ret=remove(p->cur_filename[p->cur_file_idx]);
			if(ret==0){
				ALOGW("dbg-rec room dismatch,rm old file first and then create it again");
				ret = xxCreateMyFile(p->cur_filename[p->cur_file_idx],perfilesize);   //this is needed to prevent kernel crash

			}else {
				ALOGE("dbg-rec old file OccupySize is not match the new one ,but rm old file err%d",errno);
			}
		}
		
		ret = truncate(p->cur_filename[p->cur_file_idx], 0);
		if(ret !=0) {
			ALOGW("dbg-rec t a file %s ret=%d (%d):%s",rec_filename,ret,errno,strerror(errno));
		}
		
		ret = rename(p->cur_filename[p->cur_file_idx],rec_filename);
	    if(ret !=0) {
			ALOGE("genfile re file faile %s ret=%d (%d):%s",rec_filename,ret,errno,strerror(errno));
		}


	}else{
		ALOGV("dbg-rec f a file %s size=%dM",rec_filename,perfilesize>>20);
		xxCreateMyFile(rec_filename,perfilesize);
	}
	
	//ALOGV("dbg-rec ids=[%d] perfilesize=%dM,getFileOccupySizeMb=%d,getFileSize=%d",p->cur_file_idx,(perfilesize>>20),
	//										getFileOccupySizeMb(rec_filename),getFileSize(rec_filename));
	#endif

#else   //haven't define FILE_NO_REMOVE
	ret = rename(p->cur_filename[p->cur_file_idx],rec_filename);
	if(ret !=0) {
		ALOGE("genfile re file fail ");
	}
#endif

	strcpy(p->cur_filename[p->cur_file_idx],rec_filename);

    strcpy(name,rec_filename);

	ALOGD("genfile %s",name);

	
	if(p->cur_file_idx >= p->cur_max_filenum)
    {
        p->cur_file_idx = 0;
    }
    else{
		p->cur_file_idx++;
    }
	config_set_camfileidx(0,p->cur_file_idx); 
	
    return 0;
}
#else
int dvr_factory::genfilename(char* name)
{
    char rec_filename[256];
    int rmidx=0,reidx=0;
    file_status_t *p =&mFilemanger;
    char *suffix[] = {"front", "rear","front", "rear"};
	int l32_CameraId = (360 == mCameraId)?  0: mCameraId;
    if(name==NULL)
        return -1;

    //avoid fist begin start from idx 1
    if(strlen(p->cur_filename[p->cur_file_idx])==0) {}
    else
        p->cur_file_idx++;

    if(p->cur_file_idx>=p->cur_max_filenum)
    {
        p->cur_file_idx = 0;
    }

    //config_set_camfileidx(p->cur_file_idx);
	config_set_camfileidx(mCameraId,p->cur_file_idx);

    if(strlen(p->cur_filename[p->cur_file_idx])!=0)
    {
		//remove thumb
		char buf2[CM_MAX_FILE_LEN];
    	memset(buf2,0,CM_MAX_FILE_LEN);
		sprintf(buf2, "%s%s%s%d%c%s%s%s",p->cur_filedir[0],"/",CM_THUMB_DIR,p->cur_file_idx,'-',"thumb_",suffix[l32_CameraId],EXT_PIC);
		remove(buf2);

		if(access(p->cur_filename[p->cur_file_idx], F_OK) == 0)
		{
#ifndef FILE_NO_REMOVE
        	remove(p->cur_filename[p->cur_file_idx]);
        	ALOGI("remove %s",p->cur_filename[p->cur_file_idx]);

        	memset(p->cur_filename[p->cur_file_idx],0,CM_MAX_FILE_LEN);
#else
#ifdef FILE_HOLES_SYS_SUPPORT
			ALOGW("old file exist %s",p->cur_filename[p->cur_file_idx]);
			goto FILE_CREATE;
#endif
#endif

		}
    }


    rmidx = p->cur_file_idx;
    reidx=0;
    do
    {
        reidx++;
        if(reidx >=p->cur_max_filenum)//防止死循环
            break;
        unsigned long long as = availSize(p->cur_filedir[0]);

        int afile_size = ((mDuration*encode_param.bit_rate)>>23)+(13*mDuration)/60;

        if(as >(afile_size+RESERVED_SIZE))
        {
        	ALOGW("space is avail =%lld afile=%d",as,afile_size);
            break;
        }
        else
        {
            rmidx++;
            if(rmidx >=p->cur_max_filenum)
            {
                rmidx=0;
            }
            if(strlen(p->cur_filename[rmidx])!=0)
            {
            	//remove thumb
				char buf2[CM_MAX_FILE_LEN];
		    	memset(buf2,0,CM_MAX_FILE_LEN);
				sprintf(buf2, "%s%s%s%d%c%s%s%s",p->cur_filedir[0],"/",CM_THUMB_DIR,rmidx,'-',"thumb_",suffix[l32_CameraId],EXT_PIC);
				remove(buf2);
				ALOGI("rm thumb file %s done",buf2);
				
#ifndef FILE_NO_REMOVE
				if(access(p->cur_filename[p->cur_file_idx], F_OK) == 0)
				{
                	ALOGI("rm file (%s avsize %lld afilesize %d).\n", p->cur_filename[rmidx],as,afile_size);
                	remove(p->cur_filename[rmidx]);
				}
                memset(p->cur_filename[rmidx],0,CM_MAX_FILE_LEN);
				ALOGI("rm file done");
#else
				int rmfilesize=get_file_size(p->cur_filename[rmidx]);
				 ALOGI("rm file (%s %d avsize %lld afilesize %d).\n", p->cur_filename[rmidx],rmfilesize,as,afile_size);
				if(rmfilesize < 0)
				{
					memset(p->cur_filename[rmidx],0,CM_MAX_FILE_LEN);
					continue;
				}
				#ifdef FILE_HOLES_SYS_SUPPORT
				//sendCmd2MiscWork(MISCWORK_CMD_RM,(void * )p->cur_filename[rmidx],strlen(p->cur_filename[rmidx])+1);
				if((rmfilesize>>23)>afile_size)
				{
					//this code  base on when change record time user must accept format sdcard
					int ret = rename(p->cur_filename[rmidx],p->cur_filename[p->cur_file_idx]);
					break;
				}else
				{
					sendCmd2MiscWork(MISCWORK_CMD_RM,(void * )p->cur_filename[rmidx],strlen(p->cur_filename[rmidx])+1);

				}
				#else
					sendCmd2MiscWork(MISCWORK_CMD_RM,(void * )p->cur_filename[rmidx],strlen(p->cur_filename[rmidx])+1);

				#endif
				

#endif


            }
        }
    }
    while(1);
FILE_CREATE:
    struct tm *tm=NULL;
    time_t now = getDateTime(&tm);
    sprintf(rec_filename, "%s%s%d%c%04d%02d%02d_%02d%02d%02d_%s%s",p->cur_filedir[0],"/",p->cur_file_idx,'-',tm->tm_year + 1900, tm->tm_mon + 1,
            tm->tm_mday, tm->tm_hour,tm->tm_min, tm->tm_sec,suffix[l32_CameraId],EXT_VIDEO);
#ifdef FILE_NO_REMOVE
	ALOGI("re file beginnow=%s old=%s",rec_filename,p->cur_filename[p->cur_file_idx]);
	int ret=0;

#ifdef FILE_HOLES_SYS_SUPPORT
#define FILESIZE_PER_MIN_720P (int)(64*1024*1024)
#define FILESIZE_PER_MIN_1080P (int)(128*1024*1024)

	int perfilesize=FILESIZE_PER_MIN_720P*(mDuration/60);
	int af_size =((mDuration*encode_param.bit_rate)>>23)+(13*mDuration)/60;
	if(af_size>perfilesize)
		perfilesize=af_size;
//	if(mCameraId == 0)
//	{
//		perfilesize=FILESIZE_PER_MIN_1080P*mDuration/60;
//
//	}
//	else
//	{
//		perfilesize=FILESIZE_PER_MIN_720P*mDuration/60;
//
//	}
	
	if(access(p->cur_filename[p->cur_file_idx], F_OK) == 0)
	{
		//remove(p->cur_filename[p->cur_file_idx]);
		//memset(p->cur_filename[p->cur_file_idx],0,CM_MAX_FILE_LEN);
		//ALOGI("rm file done");
		ALOGW("t a file %s ",p->cur_filename[p->cur_file_idx]);
		xxCreateMyFile(p->cur_filename[p->cur_file_idx],perfilesize);
		ret = truncate(p->cur_filename[p->cur_file_idx], 0);
		if(ret !=0) {
			ALOGW("t a file %s ret=%d (%d):%s",rec_filename,ret,errno,strerror(errno));
		}
		
		ret = rename(p->cur_filename[p->cur_file_idx],rec_filename);
	    if(ret !=0) {
			ALOGE("genfile re file faile %s ret=%d (%d):%s",rec_filename,ret,errno,strerror(errno));
		}

	}else{
		ALOGW("f a file %s %d",rec_filename,perfilesize);
		xxCreateMyFile(rec_filename,perfilesize);
		#if 0
		int fd = open(rec_filename, O_CREAT | O_RDWR, 0777);
		ret = fallocate(fd, 0x01, 0, perfilesize);
		if(ret < 0)
		{
			ALOGE("fallocate re file fail len = %d",perfilesize);
		}
		//fsync(fd);
		close(fd);
		sync();
		#endif
		sync();
	}

#else
	ret = rename(p->cur_filename[p->cur_file_idx],rec_filename);
	if(ret !=0) {
		ALOGE("genfile re file fail ");
	}

#endif

	ALOGI("re file done");
#endif
	strcpy(p->cur_filename[p->cur_file_idx],rec_filename);

    strcpy(name,rec_filename);

	ALOGD("genfile %s",name);
    return 0;
}
#endif




void CallbackForAwRecorder(void* pUserData, int msg, int param0, void* param1)
{
#ifdef REC_ENCODE

    AwRecorder* pDemoRecoder = (AwRecorder*)pUserData;

    CEDARX_UNUSE(param1);


    switch(msg)
    {
    case RECORDER_VIDEO_ENCODER_NOTIFY_OUT_BUFFER:
    {
        ALOGW("notify video out buf");
        break;
    }

    default:
    {
        ALOGW("warning: unknown callback from AwRecorder.\n");
        break;
    }
    }
#endif
    return ;
}
void dvr_factory::setCallbacks(notify_callback notify_cb,
                               data_callback data_cb,
                               data_callback_timestamp data_cb_timestamp,
                               void* user)
{
    // temp not open for users
    //if usr must use this , the cb func must process as quickly as possibale
    mNotifyCb=notify_cb;
    mDataCb=data_cb;
    mDataCbTimestamp=data_cb_timestamp;

    mCbUser=user;

}
//#define SPS_PPS_FILE 1
int dvr_factory::prepare()
{
    F_LOG;
    Mutex::Autolock locker(&mObjectLock);
#ifdef REC_ENCODE

//* set callback to recoder.
    AwRecorderSetCallback(mAwRecorder,CallbackForAwRecorder,&(callbackData));

    VideoEncodeConfig videoConfig;
    memset(&videoConfig, 0x00, sizeof(VideoEncodeConfig));
    videoConfig.nType = VIDEO_ENCODE_H264;
    videoConfig.nFrameRate = 30;
    videoConfig.nOutHeight = 720;
    videoConfig.nOutWidth = 1280;

    videoConfig.nSrcHeight = 720;
    videoConfig.nSrcWidth = 1280;

    AudioEncodeConfig audioConfig;
    audioConfig.nType = AUDIO_ENCODE_PCM_TYPE;
    audioConfig.nInChan = 2;
    audioConfig.nInSamplerate = 44100;
    audioConfig.nOutChan = 2;
    audioConfig.nOutSamplerate = 44100;
    audioConfig.nSamplerBits = 16;

    AwRecorderSetParamete(mAwRecorder, AwRecorder_SetVideoInfo, &videoConfig);
    AwRecorderSetParamete(mAwRecorder, AwRecorder_SetAudioInfo, &audioConfig);

    char pUrl[1024] = "write:///mnt/SAVE.mp4";
    AwRecorderSetParamete(mAwRecorder, AwRecorder_SetMuxerUrl, (void*)pUrl);

    int muxType = MUXER_TYPE_MOV;
    AwRecorderSetParamete(mAwRecorder, AwRecorder_SetMuxerType, &muxType);

#endif
#ifdef USE_CDX_LOWLEVEL_API
    /****************************** Define MUXER Paramerter ********************************************/

    CdxMuxerMediaInfoT media_info;
    //CdxStreamT *mp4writer = NULL;

    /****************************** Define MUXER Paramerter ********************************************/
    char name_buf[512]= {0};

	memset(name_buf,0,512);
    genfilename(name_buf);
    dvrsetvideoname(name_buf);
    LOGI("VIDEO_NAME:%s,id=%d\n", name_buf,mCameraId); 
    
	#ifdef SMART_RECORD
		__notify_cb(CAMERA_MSG_DVR_NEW_FILE,gFilemanger.cur_file_idx,mCameraId,mCbUser);
	#else
		__notify_cb(CAMERA_MSG_DVR_NEW_FILE,mFilemanger.cur_file_idx,mCameraId,mCbUser);
	#endif
	


    /******************************* Set Muxer Parameters ****************************/
    memset(&media_info, 0, sizeof(CdxMuxerMediaInfoT));
	#ifdef USE_CDX_LOWLEVEL_API_AUDIO
	AutAudioMuxInit(&media_info);
	#else
    media_info.audioNum = 0;
	#endif
    media_info.videoNum = 1;
    media_info.video.nWidth = encode_param.dst_width;
    media_info.video.nHeight = encode_param.dst_height;
    media_info.video.nFrameRate = encode_param.frame_rate;
    media_info.video.eCodeType = (VENC_CODEC_TYPE)encode_param.encode_format;

    memset(&mp4stream, 0, sizeof(CdxDataSourceT));

    mp4stream.uri = strdup(encode_param.mp4_file);
#ifdef CDX_V27
	extern CdxWriterT *CdxWriterCreat();
#endif
    if ((writer = CdxWriterCreat()) == NULL)
    {
        ALOGE("writer creat failed\n");
        return -1;
    }
    mw = (MuxerWriterT*)writer;
    strcpy(mw->file_path, mp4stream.uri);
    mw->file_mode = FD_FILE_MODE;
    int ret = MWOpen(writer);
    mux = CdxMuxerCreate(CDX_MUXER_TS, writer);
    if(mux < 0)
    {
        ALOGE("CdxMuxerCreate failed");
        return -1;
    }
	int iscard=0;
	ALOGE("CdxMuxerControl SET_IS_SDCARD_DISAPPEAR 0\n");
	CdxMuxerControl(mux,SET_IS_SDCARD_DISAPPEAR,&iscard);
    CdxMuxerSetMediaInfo(mux, &media_info);

#if FS_WRITER
    memset(&fs_cache_mem, 0, sizeof(CdxFsCacheMemInfo));

    fs_cache_mem.m_cache_size = 2*1024 * 1024; 
    fs_cache_mem.mp_cache = (cdx_uint8*)malloc(fs_cache_mem.m_cache_size);
    if (fs_cache_mem.mp_cache == NULL)
    {
        ALOGE("fs_cache_mem.mp_cache malloc failed\n");
        goto nofscache;
    }
    CdxMuxerControl(mux, SET_CACHE_MEM, &fs_cache_mem);
    fs_mode = FSWRITEMODE_CACHETHREAD;

    //fs_mode = FSWRITEMODE_DIRECT;
    CdxMuxerControl(mux, SET_FS_WRITE_MODE, &fs_mode);
nofscache:

#endif
    CdxMuxerWriteHeader(mux);
    //encode_param.pts=0;

    /******************************* Set Muxer Parameters ****************************/
#ifdef SPS_PPS_FILE
	out_file = fopen("/mnt/UDISK/640p.h264", "wb");
#endif
    //char name_test[1024]= {0};
    //sprintf(name_test,"%s.h264",name_buf);
    //out_file = fopen(name_test, "wb");
    if(mTimeridVaild==1)
    {
		stopTimer(mTimerID);
		mCurRecordPos = 0;
		setPeriodTimer(1, 0,mTimerID);//restart
	}else{
	    if(createTimer(this,&mTimerID,__timer_cb)==0)
	    {
	    	mCurRecordPos = 0;
			setPeriodTimer(1, 0,mTimerID);
			mTimeridVaild=1;
	    }else
	    {
			mTimeridVaild=0;
		}
	}
	mDvrWriteFrameNum=0;

    if(encode_param.encode_format == VENC_CODEC_H264)
    {
        VencHeaderData sps_pps_data;

        VideoEncGetParameter(pVideoEnc, VENC_IndexParamH264SPSPPS, &sps_pps_data);
        CdxMuxerWriteExtraData(mux, sps_pps_data.pBuffer, sps_pps_data.nLength, 0);
		
#ifdef SPS_PPS_FILE
		if(out_file!=NULL)
        	fwrite(sps_pps_data.pBuffer, 1, sps_pps_data.nLength, out_file);
		#endif
        //ALOGD("sps_pps_data.nLength: %d", sps_pps_data.nLength);
        //int head_num;
        //for(head_num=0; head_num<sps_pps_data.nLength; head_num++)
        //	logd("the sps_pps :%02x\n", *(sps_pps_data.pBuffer+head_num));
    }
#endif

    return 0;

}
int dvr_factory::camera_video_file_save(char *name,int filetype)
{
    int ret =-1;
    char nameback[256];
    memset(nameback,0,256);
	
	int len =strlen(name);
	
   strcpy(nameback,name);
   nameback[len-4]=0;
   if(filetype==CurRecordFileNameType_SAVE)
   {
		strcat(nameback,CM_SAVE_FILE_MARK);
   }
   else if(filetype==CurRecordFileNameType_LOCK)
   {
		strcat(nameback,CM_LOCK_FILE_MARK);
        LOGW("Lock_file_name: %s\n", nameback);
   }
   
	strcat(nameback,EXT_VIDEO);
    LOGW("video_file_name: %s\n", nameback);

    //LOGE("new name =%s",nameback);
    ret = rename(name,nameback);
    if(ret ==0) {
	//LOGE("video%d rename rt =%s success",g_cm_run_status.videono,nameback);
    }
    return ret;
}
int dvr_factory::camera_change_file_name(int filetype)
{
    int ret=0;

	#ifdef SMART_RECORD
		Mutex::Autolock locker(&gfileLock);
	    file_status_t *p =&gFilemanger;
	#else
		file_status_t *p =&mFilemanger;	
	#endif
	
    int cur_idx = p->cur_file_idx;
    int pre_idx = cur_idx>0?cur_idx-1:p->cur_max_filenum-1;
    //save cur file and pre file
    ret = camera_video_file_save(p->cur_filename[cur_idx],filetype);
    if(ret == 0) {
        memset(p->cur_filename[cur_idx],0,CM_MAX_FILE_LEN);
		p->cur_dir_file_num--;
    }
#if 0
    ret =  camera_video_file_save(p->cur_filename[pre_idx],filetype);
    if(ret == 0) {
        memset(p->cur_filename[pre_idx],0,CM_MAX_FILE_LEN);
		p->cur_dir_file_num--;
    }
#endif
    return 0;
}

int dvr_factory::closemux()
{
#ifdef REC_ENCODE

    AwRecorderStop(mAwRecorder);

#endif
#ifdef USE_CDX_LOWLEVEL_API
    ALOGW("close mux ");

    if(mux==NULL){
		LOGD("closemux mux null");
        return 0;
    }
#ifdef SPS_PPS_FILE

	if(out_file!=NULL)
	{
	    fclose(out_file);
	    out_file = NULL;
	}
	#endif
	if(storage_state==1)
	{
	    int result = CdxMuxerWriteTrailer(mux);
	    if (result)
	    {
	        LOGD("CdxMuxerWriteTrailer failed");
	        return -1;
	    }else
	    {
			LOGD("CdxMuxerWriteTrailer success");
		}
	}else{
	//CdxMuxerWriteTrailer(mux);//some problem so comment
		LOGD("closemux storage_state !=1");

	}

    CdxMuxerClose(mux);
	
    mux=NULL;

    if(mp4stream.uri)
    {
        free(mp4stream.uri);
        mp4stream.uri = NULL;
    }
    if (writer)
    {
        MWClose(writer);

        CdxWriterDestroy(writer);
		F_LOG;
        writer = NULL;
        mw = NULL;
    }
	if(fs_cache_mem.mp_cache!=NULL){
		free(fs_cache_mem.mp_cache); 
		fs_cache_mem.mp_cache=NULL;
		}
	LOGD("closemux done");
    return 0;
#endif

}
int dvr_factory::stop()
{
    Mutex::Autolock locker(&mObjectLock);
    if(rtsp_started)
    {
        stop_rtsp();
        rtsp_started = 0;
    }

    ALOGW("stop camera id %d", mCameraId);
    mHardware->disableMsgType(CAMERA_MSG_ALL_MSGS);
    mHardware->stopRecording();
    mHardware->stopPreview();
    mHardware->cancelPicture();

    pthread_mutex_lock(&mRecordFileMutex);

    if (dvrstatus == RECORD_STATE_NULL)
    {
        LOGE("error state of capture thread, %s", __FUNCTION__);
        pthread_mutex_unlock(&mRecordFileMutex);
        return EINVAL;
    }

    if (dvrstatus == RECORD_STATE_PAUSED)
    {
        LOGW("capture thread has already paused");
        pthread_mutex_unlock(&mRecordFileMutex);
        return NO_ERROR;
    }

    dvrstatus = RECORD_STATE_PAUSED;
    pthread_cond_signal(&mRecordFileCond);

    pthread_mutex_unlock(&mRecordFileMutex);

    return NO_ERROR;

}
#ifdef USE_CDX_LOWLEVEL_API_AUDIO

int dvr_factory::AudioRecStop()
{

	pthread_mutex_lock(&mAudioEncMutex);

   if (mAudioThreadState == RECORD_STATE_NULL)
   {
	   LOGE("error state of capture thread, %s", __FUNCTION__);
	   pthread_mutex_unlock(&mAudioEncMutex);
	   return EINVAL;
   }

   if (mAudioThreadState == RECORD_STATE_PAUSED)
   {
	   LOGW("capture thread has already paused");
	   pthread_mutex_unlock(&mAudioEncMutex);
	   return NO_ERROR;
   }

   mAudioThreadState = RECORD_STATE_PAUSED;
   pthread_cond_signal(&mAudioEncCond);
	
   pthread_mutex_unlock(&mAudioEncMutex);
   return NO_ERROR;
}
int dvr_factory::AudioRecStart()
{
	F_LOG
    pthread_mutex_lock(&mAudioEncMutex);

    if (mAudioThreadState == RECORD_STATE_NULL)
    {
        LOGE("error state of capture thread, %s", __FUNCTION__);
        pthread_mutex_unlock(&mAudioEncMutex);
        return EINVAL;
    }

    if (mAudioThreadState == RECORD_STATE_STARTED)
    {
        LOGW("capture thread has already started");
        pthread_mutex_unlock(&mAudioEncMutex);
        return NO_ERROR;
    }
	F_LOG

    // singal to start capture thread
    mAudioThreadState = RECORD_STATE_STARTED;
    pthread_cond_signal(&mAudioEncCond);
    pthread_mutex_unlock(&mAudioEncMutex);

    return NO_ERROR;

}
#endif
int dvr_factory::startPriview(struct view_info vv)
{
    struct src_info srcinfo= {recordwith,recordheith,0};
    Mutex::Autolock locker(&mObjectLock);

    mHardware->setPreviewParam(vv,srcinfo,0,3);
    mHardware->enableMsgType(CAMERA_MSG_PREVIEW_FRAME);

    return NO_ERROR;

}
int dvr_factory::stopPriview()
{
    mHardware->disableMsgType(CAMERA_MSG_PREVIEW_FRAME);

    return NO_ERROR;

}
int dvr_factory::startRecord()
{
    //struct src_info srcinfo= {recordwith,recordheith,0};
    Mutex::Autolock locker(&mObjectLock);
	F_LOG;
#if 0
//#ifdef RECORD_ALONE

    pthread_mutex_lock(&mRecordFileMutex);

    if (dvrstatus == RECORD_STATE_NULL)
    {
        LOGE("error state of capture thread, %s", __FUNCTION__);
        pthread_mutex_unlock(&mRecordFileMutex);
        return EINVAL;
    }

    if (dvrstatus == RECORD_STATE_STARTED)
    {
        LOGW("capture thread has already started");
        pthread_mutex_unlock(&mRecordFileMutex);
        return NO_ERROR;
    }

    // singal to start capture thread
    dvrstatus = RECORD_STATE_STARTED;
    pthread_cond_signal(&mRecordFileCond);
    pthread_mutex_unlock(&mRecordFileMutex);

    int result = mHardware->startRecording();
#endif
	encode_param.encode_frame_num=0;
	mDvrWriteFrameNum =0;
    //enc_parm_init(recordwith,recordheith,recordwith,recordheith,5,30,VENC_CODEC_H264);
	
	char bufname[512];
	
	memset(bufname,0,512);
	config_get_curfiledir(mCameraId,bufname);
	bool rt=false;
	if(bufname[0]==0)
	{
		ALOGV("#######find the pat is NULL");
		rt=initFileListDir(MOUNT_POINT);
	}
	else
		rt=initFileListDir(bufname);
	
	if(rt)
	{
		mNeedNewFile = 1;
		pthread_mutex_lock(&mRecordFileMutex);

	    if (dvrstatus == RECORD_STATE_NULL)
	    {
	        LOGE("startRecord error state of capture thread, %s", __FUNCTION__);
	        pthread_mutex_unlock(&mRecordFileMutex);
	        //return EINVAL;
	    }

	    if (dvrstatus == RECORD_STATE_STARTED)
	    {
	        LOGW("startRecord capture thread has already started");
	        pthread_mutex_unlock(&mRecordFileMutex);
	        //return NO_ERROR;
	    }

	    // singal to start capture thread
	    dvrstatus = RECORD_STATE_STARTED;
	    pthread_cond_signal(&mRecordFileCond);
	    pthread_mutex_unlock(&mRecordFileMutex);
	
		mHardware->enableMsgType(CAMERA_MSG_VIDEO_FRAME);//enable recorder
		int result = mHardware->startRecording();
		mVideoRecordQueueErr=0;
		mAudioRecordQueueErr=0;
		}
	else
		return INVALID_OPERATION;
	
#ifdef USE_RECORD_AUDIO_API
	AudioRecStart();//编码线程启动
	if(mAudioCap!=NULL){
		mAudioCap->startCapture(mAudioHdl);//采集线程启动
	}
#endif
    return NO_ERROR;

}
int dvr_factory::stopRecord()
{
	F_LOG;

	mHardware->disableMsgType(CAMERA_MSG_VIDEO_FRAME);//enable recorder
	//closemux();
	
	pthread_mutex_lock(&mRecordFileMutex);
	
	   if (dvrstatus == RECORD_STATE_NULL)
	   {
		   LOGE("error state of capture thread, %s", __FUNCTION__);
		   pthread_mutex_unlock(&mRecordFileMutex);
		   return EINVAL;
	   }
	
	   if (dvrstatus == RECORD_STATE_PAUSED)
	   {
		   LOGW("capture thread has already paused");
		   pthread_mutex_unlock(&mRecordFileMutex);
		   return NO_ERROR;
	   }
	
	   dvrstatus = RECORD_STATE_PAUSED;
	   pthread_cond_signal(&mRecordFileCond);
	
	   pthread_mutex_unlock(&mRecordFileMutex);

	encode_param.encode_frame_num=0;
	//mDvrWriteFrameNum =0;
#ifdef USE_RECORD_AUDIO_API
	if(mAudioCap!=NULL)
		mAudioCap->stopCapture(mAudioHdl);
	AudioRecStop();
#endif
    if(mTimeridVaild==1)
    {
    	ALOGW("stop timer cid=%d",mCameraId);
		stopTimer(mTimerID);
		mCurRecordPos = 0;
    }
    return NO_ERROR;

}

int dvr_factory::takePicture()
{
    mHardware->enableMsgType(CAMERA_MSG_COMPRESSED_IMAGE);
    mHardware->takePicture();
    return NO_ERROR;

}

int dvr_factory::start()
{
#ifdef REC_ENCODE

    AwRecorderStart(mAwRecorder);

#endif
    F_LOG;
    Mutex::Autolock locker(&mObjectLock);
#ifndef RECORD_ALONE
    pthread_mutex_lock(&mRecordFileMutex);

    if (dvrstatus == RECORD_STATE_NULL)
    {
        LOGE("error state of capture thread, %s", __FUNCTION__);
        pthread_mutex_unlock(&mRecordFileMutex);
        return EINVAL;
    }

    if (dvrstatus == RECORD_STATE_STARTED)
    {
        LOGW("capture thread has already started");
        pthread_mutex_unlock(&mRecordFileMutex);
        return NO_ERROR;
    }

    // singal to start capture thread
    dvrstatus = RECORD_STATE_STARTED;
    pthread_cond_signal(&mRecordFileCond);
    pthread_mutex_unlock(&mRecordFileMutex);
#endif
    String8 params(mHardware->getParameters().flatten());

    CameraParameters p(params);
    p.setPreviewSize(recordwith,recordheith);
    p.setVideoSize(recordwith,recordheith);
	const char *format=p.get(KEY_CAPTURE_FORMAT);
	int fmt=atoi(format);
	//if(format == V4L2_PIX_FMT_H264)
	//{	
		mCaptureFmt=fmt;
	//}
    mHardware->setParameters(p);

    //mHardware->enableMsgType(CAMERA_MSG_VIDEO_FRAME); //fktodo

    int result = mHardware->startPreview();
	#ifndef RECORD_ALONE
    result += mHardware->startRecording();
#endif
    return result;

}
int dvr_factory::enc_de_init()
{
    F_LOG;
    //Mutex::Autolock locker(&mObjectLock);
#ifdef USE_CDX_LOWLEVEL_API
    int result = VideoEncUnInit(pVideoEnc);
    if( result )
    {
        loge("VideoEncUnInit error result=%d...\n",result);
    }

    VideoEncDestroy(pVideoEnc);
    pVideoEnc = NULL;
    if(rec_enc_baseConfig.memops)
    {
        CdcMemClose(rec_enc_baseConfig.memops);
        rec_enc_baseConfig.memops = NULL;
    }
#endif
#ifdef USE_CDX_LOWLEVEL_API_AUDIO
	AutAudioEncDeinit();

#endif

    return 0;
}
int dvr_factory::genThumbData(char* dataPtr,V4L2BUF_t *outbuf)
{
	int *ptp=(int *)dataPtr;
	char *ptpc=(char *)dataPtr;
	VencInputBuffer *p=(VencInputBuffer *)(ptpc+4);
	
	V4L2BUF_t * pbuf = (V4L2BUF_t*)*ptp;
	int sca_w=pbuf->width/4;
	int sca_h=pbuf->height/4;
#ifdef USE_CDX_NEW_MEM_API
	CdcMemFlushCache(mCameraMemops,(void*)pbuf->addrVirY, recordheith*recordwith*3/2);		
#else	
	ion_flush_cache((void*)pbuf->addrVirY, recordheith*recordwith*3/2);
#endif			
	g2d_func_scale((unsigned char *)pbuf->addrPhyY,recordwith,recordheith,(unsigned char *)mG2dAddrPhy,sca_w,sca_h);
#ifdef USE_CDX_NEW_MEM_API
		CdcMemFlushCache(mCameraMemops,(void*)mG2dAddrVir,sca_w*sca_h*4);		
#else	
		ion_flush_cache((void*)mG2dAddrVir, sca_h*sca_w*4);
#endif	

	memcpy(outbuf,pbuf,sizeof(V4L2BUF_t));

	outbuf->width = sca_w;
	outbuf->height = sca_h;
	outbuf->addrVirY = mG2dAddrVir;
	outbuf->addrPhyY = mG2dAddrPhy;
	
	return 0;

}

int dvr_factory::g2d_func_open()
{
	// open MP driver
	mG2DHandle = open("/dev/g2d", O_RDWR, 0);
	if (mG2DHandle < 0)
	{
		LOGE("open /dev/g2d failed");
		return -1;
	}
	mCameraMemops = MemAdapterGetOpsS();	 
	if(mCameraMemops == NULL)
	{ 
		LOGE("camera MemAdapterGetOpsS failed\n");		  
	}    
	
	int ret = CdcMemOpen(mCameraMemops);

	if (ret < 0)
	{
		LOGE("ion_alloc_open failed");
		//goto END_ERROR;
	}

	mG2dAddrVir = (int)CdcMemPalloc(mCameraMemops, G2D_FUNC_BUFFER_SIZE);
	mG2dAddrPhy = (unsigned int)CdcMemGetPhysicAddressCpu(mCameraMemops,(void*)mG2dAddrVir);
	
	LOGV("open /dev/g2d OK");
	return 0;
}
int dvr_factory::g2d_func_close()
{
	if(mG2DHandle != NULL)
	{
		close(mG2DHandle);
		mG2DHandle = NULL;
	}
	
	if(mG2dAddrVir != NULL)
	{
		CdcMemPfree(mCameraMemops,(void*)mG2dAddrVir);
		mG2dAddrPhy = 0;
	}
	return 0;

}
int dvr_factory::g2d_func_scale(unsigned char *src, int src_width, int src_height, unsigned char *dst, int dst_width, int dst_height)
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
		scale.dst_image.format =G2D_FMT_RGB565;// G2D_FMT_PYUV420UVC;
		scale.dst_image.pixel_seq = G2D_SEQ_NORMAL;
		scale.dst_rect.x = 0;
		scale.dst_rect.y = 0;
		scale.dst_rect.w = dst_width;
		scale.dst_rect.h = dst_height;
		scale.color = 0xff;
		scale.alpha = 0xff; 
	
		if(ioctl(mG2DHandle, G2D_CMD_STRETCHBLT, &scale)<0){

			ALOGE("-------------g2d scalar fail fd=%d",mG2DHandle);
			return -1;
		}
		return 0;
	}


int dvr_factory::recordInit(const char *name)
{
#ifdef RECORD_ALONE
	if(mRecordInitDone ==1){
		ALOGW("recordInit ALREADY INIT");
		return 0;
	}
// init record buffer queue
	if(mUseMsgQ)
	{
		char qname[128];
		sprintf(qname,"/q_cid_%d", gettid());
		int ret = msgQCreate(&mQueueRecordFile,qname,192,sizeof(struct mymesg),1);
		if(ret !=0)
		{
			ALOGW("%s queue create fail",qname);
		}else
		{
			ALOGW("%s queue create %d",qname,mQueueRecordFile);
		}
	}else
	{
		OSAL_QueueCreate(&mQueueBufferRecordFile, 192);
	}

// init record thread
    mRecordFileThread = new RecordFileThread(this);
    pthread_mutex_init(&mRecordFileMutex, NULL);
    pthread_cond_init(&mRecordFileCond, NULL);
    dvrstatus=RECORD_STATE_PAUSED;

    mRecordFileThread->startThread();

// init audio record thread
#ifdef USE_CDX_LOWLEVEL_API_AUDIO
	OSAL_QueueCreate(&mQueueBufferAudioEnc, 96);

	mAudioEncThread = new AudioEncThread(this);
	pthread_mutex_init(&mAudioEncMutex, NULL);
	pthread_cond_init(&mAudioEncCond, NULL);
	mAudioThreadState=RECORD_STATE_PAUSED;

	mAudioEncThread->startThread();
#endif

#if 0

	OSAL_QueueCreate(&mQueueBufferMiscWork, 96);

	mMiscWorkThread = new MiscWorkThread(this);
	pthread_mutex_init(&mMiscWorkMutex, NULL);
	pthread_cond_init(&mMiscWorkCond, NULL);
	mMiscWorkThreadState=RECORD_STATE_PAUSED;

	mMiscWorkThread->startThread();
#endif
    int cycltime = config_get_cyctime(mCameraId);
#else
    int cycltime;

    int cx,cy;

    bool rt=initializeDev(name);
    ALOGV("######################test init rt= %d  cameraid=%s#########",rt,name);
	
    if(!rt)
    {
        ALOGV("##########dev init fail repeat");
        return -1;
    }
    cx  = config_get_weith(mCameraId);
    cy = config_get_heigth(mCameraId);
    cycltime = config_get_cyctime(mCameraId);



    recordwith  = cx;
    recordheith = cy;
	#endif
    //dvr.SetDataCB(usr_h264datacb,&dvr);
    if(0 == mCameraId)
    {
        enc_parm_init(recordwith,recordheith,recordwith,recordheith,8,25,VENC_CODEC_H264);
    }
    else
    {
        enc_parm_init(recordwith,recordheith,recordwith,recordheith,7,30,VENC_CODEC_H264);
    }

    setDuration(cycltime*60);
	mRecordInitDone = 1;
    return 0;


}
int dvr_factory::enc_parm_init(int sw,int sh,int dw,int dh,unsigned int bitrate,int framerate,VENC_CODEC_TYPE type)
{
    F_LOG;
    //Mutex::Autolock locker(&mObjectLock);

    memset(&encode_param, 0, sizeof(encode_param));
    encode_param.src_width = sw;
    encode_param.src_height = sh;
    encode_param.dst_width = dw;
    encode_param.dst_height = dh;
    encode_param.bit_rate = bitrate* 1024 * 1024;
    encode_param.frame_rate = framerate;
    encode_param.maxKeyFrame = framerate;
    encode_param.encode_format = type;
    encode_param.encode_frame_num = 0;

	#ifdef USE_CDX_LOWLEVEL_API_AUDIO
	memset(&mAudioEncConfig, 0x00, sizeof(AudioEncConfig));
	mAudioEncConfig.nInChan = 2;
	mAudioEncConfig.nInSamplerate = 44100;
	mAudioEncConfig.nOutChan = 2;
	mAudioEncConfig.nOutSamplerate = 44100;
	mAudioEncConfig.nSamplerBits = 16;
	mAudioEncConfig.nType = AUDIO_ENCODER_AAC_TYPE;
	mAudioEncConfig.nFrameStyle = 1;
	#endif
	
#ifdef USE_CDX_LOWLEVEL_API
    setparm(); ///fktodo
#endif
    return 0;

}
int dvr_factory::dvrsetvideoname(char* name)
{
    F_LOG;

    int ret =-1;
    if(name!=NULL)
    {
        strcpy((char*)encode_param.mp4_file,name);//		"write:///mnt/UDISK/720p_h264.mp4");
        ret =0;
    }
    return ret;
}

dvr_factory::dvr_factory():mux(NULL),usrDatCb(NULL),
    mNotifyCb(NULL),
    mDataCb(NULL),
    mDataCbTimestamp(NULL),
    mCurRecordFileNameType(CurRecordFileNameType_NORMAL),
    mCurRecordStat(RecordStat_NORMAL),
    mCbUser(NULL),mNeedThumbpic(0),mAudioflag(0),
    mAudioCap(NULL),mAudioHdl(0),mCurRecordLockOps(LockOps_CreateNewAfterClose)
    ,mUseMsgQ(1),mRecordInitDone(0)
{

    istestrtsp =0;
	storage_state=0;
	fs_mode= FSWRITEMODE_DIRECT;
	mNeedNewFile = 0;
	FileNewStampBase = 0;
	mCurRecordPos = 0;
	mTimeridVaild = 0;

    rtsp_started = 0;
    can_deliver_rtspdata =0;
	//aut_set_disp_rotate_mode(0);

    mcd=HwDisplay::getInstance();
    //mcd->tvout(3,720,480);
	g2d_func_open();

    int ret = pthread_mutex_init(&mMutex, NULL);
	mUseMsgQ = 0;//when 1 use posix mqueue ,0 for osal msgq
#ifndef RECORD_ALONE
// init record buffer queue
	if(mUseMsgQ)
	{
		char qname[128];
		sprintf(qname,"/q_cid_%d", gettid());
		int ret = msgQCreate(&mQueueRecordFile,qname,192,sizeof(struct mymesg),1);
		if(ret !=0)
		{
			ALOGW("%s queue create fail",qname);
		}else
		{

			ALOGW("%s queue create %d",qname,mQueueRecordFile);
		}
	}else
	{
		OSAL_QueueCreate(&mQueueBufferRecordFile, 192);
	}

// init record thread
    mRecordFileThread = new RecordFileThread(this);
    pthread_mutex_init(&mRecordFileMutex, NULL);
    pthread_cond_init(&mRecordFileCond, NULL);
    dvrstatus=RECORD_STATE_PAUSED;

    mRecordFileThread->startThread();

// init audio record thread
#ifdef USE_CDX_LOWLEVEL_API_AUDIO
	OSAL_QueueCreate(&mQueueBufferAudioEnc, 96);

	mAudioEncThread = new AudioEncThread(this);
	pthread_mutex_init(&mAudioEncMutex, NULL);
	pthread_cond_init(&mAudioEncCond, NULL);
	mAudioThreadState=RECORD_STATE_PAUSED;

	mAudioEncThread->startThread();
#endif
#if 0
	OSAL_QueueCreate(&mQueueBufferMiscWork, 96);

	mMiscWorkThread = new MiscWorkThread(this);
	pthread_mutex_init(&mMiscWorkMutex, NULL);
	pthread_cond_init(&mMiscWorkCond, NULL);
	mMiscWorkThreadState=RECORD_STATE_PAUSED;

	mMiscWorkThread->startThread();
#endif
#endif

}
#ifdef ADAS_ENABLE

bool dvr_factory::enableAdas()
{
    ALOGV("enableAdas");
    if(mHardware==NULL)
    {
        return false;
    }
    if (mHardware->sendCommand(CAMERA_CMD_START_ADAS_DETECTION, 0, 0) != NO_ERROR)
    {
        ALOGE("disable watermark failed");
        return false;
    }
    usleep(50000);
    return true;
}
bool dvr_factory::disableAdas()
{
    ALOGV("disableAdas");
    if(mHardware==NULL)
    {
        return false;
    }

    if (mHardware->sendCommand(CAMERA_CMD_STOP_ADAS_DETECTION, 0, 0) != NO_ERROR)
    {
        ALOGE("disable watermark failed");
        return false;
    }
    return true;
}
status_t dvr_factory::setAdasParameters(int key,int value)
{
    if(mHardware==NULL)
    {
        return false;
    }
	return mHardware->setAdasParameters(key,value);


}
bool dvr_factory::setAdasCallbacks(adas_data_callback adas_data_cb)
{
    if(mHardware==NULL)
    {
        return false;
    }
	
	mHardware->setAdasCallbacks(adas_data_cb);
	return true;


}
#endif
bool dvr_factory::enableWaterMark()
{
    ALOGV("enableWaterMark");
    if(mHardware==NULL)
    {
        return false;
    }
    if (mHardware->sendCommand(CAMERA_CMD_START_WATER_MARK, 0, 0) != NO_ERROR)
    {
        ALOGE("disable watermark failed");
        return false;
    }
    usleep(50000);
    return true;
}
bool dvr_factory::disableWaterMark()
{
    ALOGV("disableWaterMark");
    if(mHardware==NULL)
    {
        return false;
    }

    if (mHardware->sendCommand(CAMERA_CMD_STOP_WATER_MARK, 0, 0) != NO_ERROR)
    {
        ALOGE("disable watermark failed");
        return false;
    }
    return true;
}

status_t dvr_factory::setWaterMarkMultiple(const char *str)
{
    ALOGV("setWaterMarkMultiple");
    if (mHardware == NULL || str == NULL) 
		return BAD_VALUE;

    ALOGV("setWaterMarkMultiple(%s)", str);
    String8 str8(str);// = new String8(str)
    return mHardware->setWaterMarkMultiple(str8,WATER_MARK_DISP_MODE_VIDEO_AND_PREVIEW);

}

bool dvr_factory::initializeDev(const char *name)
{
    mHardware = new CameraHardwareInterface(name);
    extern camera_module_t HAL_MODULE_INFO_SYM;

    int mNumberOfCameras = HAL_MODULE_INFO_SYM.get_number_of_cameras();

    //int mCameraId=0;
	//if(atoi(name) == 360)
	//{
	mCameraId=	atoi(name);

	//}
    struct camera_info info;
    if (HAL_MODULE_INFO_SYM.get_camera_info(0, &info) != OK)
    {
        ALOGE("Invalid camera id %d", mCameraId);
        return false;
    }

    int res = mHardware->initialize(&HAL_MODULE_INFO_SYM.common);

	
    if (res != OK)
    {
        ALOGE("%s: Camera %d: unable to initialize device: %s (%d)",
              __FUNCTION__, mCameraId, strerror(-res), res);
        mHardware.clear();
        return false;
    }

    // Enable zoom, error, focus, and metadata messages by default
    mHardware->enableMsgType(CAMERA_MSG_ERROR | CAMERA_MSG_ZOOM | CAMERA_MSG_FOCUS |
                             /* MOTION_DETECTION_ENABLE */
#ifdef ADAS_ENABLE
                             CAMERA_MSG_ADAS_METADATA |	/* ADAS_ENABLE */
#endif
                             CAMERA_MSG_PREVIEW_METADATA | CAMERA_MSG_FOCUS_MOVE);
#ifdef ADAS_ENABLE
		//mHardware->setAdasCallbacks(adasDataCallback);
#endif

    mHardware->setCallbacks(__notify_cb,
                            __data_cb,
                            __data_cb_timestamp,
                            (void *)this);


    //for hardware codec
    mHardware->sendCommand(CAMERA_CMD_SET_CEDARX_RECORDER, 0, 0);


    return true;
}
bool dvr_factory::uninitializeDev()
{

    // Release the hardware resources.
    mHardware->release();
    mHardware.clear();
    return true;

}

dvr_factory::~dvr_factory()
{

    F_LOG;
    if (mRecordFileThread != NULL)
    {
        mRecordFileThread->stopThread();
        pthread_cond_signal(&mRecordFileCond);
        mRecordFileThread.clear();
        mRecordFileThread = 0;
    }

    pthread_mutex_destroy(&mRecordFileMutex);
    pthread_cond_destroy(&mRecordFileCond);
	if(mUseMsgQ)
	{
		int ret = msgQdestory(mQueueRecordFile);

	}else
    OSAL_QueueTerminate(&mQueueBufferRecordFile);
	
#ifdef USE_CDX_LOWLEVEL_API_AUDIO

    if (mAudioEncThread != NULL)
    {
        mAudioEncThread->stopThread();
        pthread_cond_signal(&mAudioEncCond);
        mAudioEncThread.clear();
        mAudioEncThread = 0;
    }
	

    pthread_mutex_destroy(&mAudioEncMutex);
    pthread_cond_destroy(&mAudioEncCond);	
	OSAL_QueueTerminate(&mQueueBufferAudioEnc);
#endif

#if 0
    if (mMiscWorkThread != NULL)
    {
        mMiscWorkThread->stopThread();
        pthread_cond_signal(&mMiscWorkCond);
        mMiscWorkThread.clear();
        mMiscWorkThread = 0;
    }

    pthread_mutex_destroy(&mMiscWorkMutex);
    pthread_cond_destroy(&mMiscWorkCond);	
	OSAL_QueueTerminate(&mQueueBufferMiscWork);
#endif
    pthread_mutex_destroy(&mMutex);
	g2d_func_close();

}
//int 1 for normal  2:lock  3:save
int dvr_factory::setCurRecordFileLock()
{
	mCurRecordFileNameType=CurRecordFileNameType_LOCK;
	return 0;
}

int dvr_factory::setCurRecordFileSave()
{
	mCurRecordFileNameType=CurRecordFileNameType_SAVE;	
	return 0;
}
int dvr_factory::setCurRecordFileNormal()
{
	mCurRecordFileNameType=CurRecordFileNameType_NORMAL;
	return 0;
}


int dvr_factory::setCurRecordLockOps(int ops)
{
//#define LockOps_CreateNewNow 1
//#define LockOps_CreateNewAfterClose 2
	if((ops!=LockOps_CreateNewNow)&&(ops!=LockOps_CreateNewNow))
		return -1;
	mCurRecordLockOps=ops;	
	return 0;
}

//for 1 :normal for 2:stop car record
int dvr_factory::setCurRecordStat(int stat)
{
	//need check parm
	
	mCurRecordStat=stat;

}


void dvr_factory::dataCallback(int32_t msgType,
                               char* dataPtr, camera_frame_metadata_t *metadata, void* user)
{

    if(msgType&CAMERA_MSG_COMPRESSED_IMAGE)
    {
        ALOGV("store pic us time %p us size=%d",dataPtr,metadata->number_of_faces);
        long long time1= GetNowUs();
        int fd=generatepicFile();
        ALOGV("store pic us time us fd=%d",fd);
        int len=metadata->number_of_faces;
		int res;
#ifdef	FILE_HOLES_SYS_SUPPORT
		if (res = fallocate(fd, FALLOC_FL_KEEP_SIZE, 0, ALIGN_4K(len))) {
			fprintf(stderr, "fallocate line %d len=%d %s\n",__LINE__, ALIGN_4K(len),strerror(errno));
		}
		fsync(fd);
#endif
        int ret=savePicture(dataPtr,len,fd);
        long long time2= GetNowUs();
        ALOGV("store pic us time %lld us len=%d",time2-time1,len);
		__notify_cb(CAMERA_MSG_COMPRESSED_IMAGE,ret,len,mCbUser);

    }
}
//#define ENC_BENMARK
status_t dvr_factory::dataCallbackTimestamp(nsecs_t timestamp,
        int32_t msgType, char* dataPtr, void* user)
{

    int ret;
	int isKeyframe=0;
    CdxMuxerPacketT pkt;

    memset(&pkt, 0, sizeof(CdxMuxerPacketT));
    if(can_deliver_rtspdata)
    {
        //CdxMuxerPacketT *pt=push_ringbuf(&rtspQbuf);
        encode_rtsp((V4L2BUF_t *)dataPtr,NULL,NULL,user,0);
    }

	if(mCaptureFmt != V4L2_PIX_FMT_H264)
	{
	    //hw enc right now~!
	    #ifdef ENC_BENMARK
	    static int cnttt=0;
		static long long timeends=0;
		
	    long long timestart= GetNowUs();
		
	    ret=encode((V4L2BUF_t *)dataPtr,(char*)&pkt,NULL,user);
		long long timeend= GetNowUs();
		cnttt++;
		timeends+=timeend-timestart;
		ALOGV("store pic us time %lld us avl=%lld",timeend-timestart,timeends/cnttt);
		#else
		ret=encode((V4L2BUF_t *)dataPtr,(char*)&pkt,NULL,user);
		#endif
		
	    if(ret<0)
	    {
	        LOGW("encode fail");
	        return -1;
	    }
		if(ret==1)
			isKeyframe =1;
	}
	else
	{
		CdxMuxerPacketT *ppkt=(CdxMuxerPacketT *)&pkt;
		char *ptp=(char *)dataPtr;
		VencInputBuffer *p=(VencInputBuffer *)(ptp+4);		
		ppkt->buflen = p->nFlag;
		LOGW("h264 no encode ");
		if ((ppkt->buf = (char*)malloc(ppkt->buflen)) == NULL)
		{
			ALOGE("pkt.buf malloc failed in	 frame\n");
			return -1;
		}
		//F_LOG
		memcpy(ppkt->buf, p->pAddrVirY, ppkt->buflen);

		//ALOGE("encode p=%p,len=%d pk pts=%lld,inputpts=%lld,outpts=%lld",ppkt->buf,ppkt->buflen,ppkt->pts,p->nPts,outputBuffer.nPts);
		ppkt->pts = p->nPts/1000; // us should be change to ms
		ppkt->duration = 1.0/ encode_param.frame_rate * 1000;
		ppkt->type = 0;
		ppkt->streamIndex = 0;


	}


    if(usrDatCb!=NULL)
        usrDatCb(0,(char*)pkt.buf,pkt.buflen,mCbUserDat);

    struct mymesg *mesg=NULL;
	struct mymesg amesg;
	if(mUseMsgQ)
	{
		mesg=&amesg;

	}
	else
	{
		mesg=(struct mymesg *)malloc(sizeof(struct mymesg));
		
	    if(mesg==NULL)
	    {
	        ALOGE("encode fail");
	        return -1;
	    }
	}

    memcpy(&mesg->mesg_data[0],&pkt,sizeof(CdxMuxerPacketT));//blank
    //mesg->mesg_type = CDX_MEDIA_AUDIO;
    mesg->timestamp = timestamp;
    mesg->user=user;
    mesg->mesg_len = sizeof(CdxMuxerPacketT);
    //
    mesg->mesg_type = msgType;
	mesg->iskeyframe=isKeyframe;
	
	//CdxMuxerPacketT *ppkt=(CdxMuxerPacketT *)&mesg->mesg_data[0];
	//   ALOGD("dataCallbackTimestamp pts=%lld dur=%lld success",ppkt->pts,ppkt->duration);

    //LOGD("data time cb -----vy=%p idx=%d refcnt=%d",pb->addrVirY,pb->index, pb->refCnt);
    if(mUseMsgQ)
    {
     	ret = msgQSend(mQueueRecordFile,(char * )mesg,(sizeof(struct mymesg)),0);
	    if (ret != 0)
	    {
	    	mVideoRecordQueueErr++;
			if((mVideoRecordQueueErr%30)==1)//FIRST AND 30ERROR REPORT
	        	LOGW("record queue full  id=%d",mCameraId);
	        free(pkt.buf);

	        pthread_mutex_lock(&mRecordFileMutex);
	        pthread_cond_signal(&mRecordFileCond);
	        pthread_mutex_unlock(&mRecordFileMutex);
	        return FAILED_TRANSACTION;

	    }else
	    {
	    	//ALOGW("send success ----------");
			//msgQGetMsgCnt(mQueueRecordFile);
		}

	}else{
	    ret = OSAL_Queue(&mQueueBufferRecordFile, mesg);
	    if (ret != 0)
	    {
	    	mVideoRecordQueueErr++;
				if((mVideoRecordQueueErr%30)==1)//FIRST AND 30ERROR REPORT
	        LOGW("record queue full  id=%d",mCameraId);

	        if(!pkt.buf){
	        	free(pkt.buf);
	        	pkt.buf = NULL;
	        }

	        if(!mesg){
	        	free(mesg);
	        	mesg = NULL;
	        }
	        
	        pthread_mutex_lock(&mRecordFileMutex);
	        pthread_cond_signal(&mRecordFileCond);
	        pthread_mutex_unlock(&mRecordFileMutex);
	        return FAILED_TRANSACTION;

	    }
	}
    pthread_mutex_lock(&mRecordFileMutex);
    pthread_cond_signal(&mRecordFileCond);
    pthread_mutex_unlock(&mRecordFileMutex);
    //LOGV("record queue inform a pack");

    return NO_ERROR;
}
//}
int dvr_factory::setCrop(int newzoom,int maxzoom)
{
    String8 params(mHardware->getParameters().flatten());
	LOGV("dvr_factory::setCrop enw=%d max=%d",newzoom,maxzoom);
    CameraParameters p(params);
	p.set(CameraParameters::KEY_ZOOM, newzoom);

    mHardware->setParameters(p);

}
void dvr_factory::__timer_cb(sigval_t sig)
{
	dvr_factory *__this =
			   static_cast<dvr_factory *>(sig.sival_ptr);
	//ALOGV("__timer_cb-----");
	if(__this!=NULL)
	{
		__this->mCurRecordPos++;
		if(((__this->mCurRecordPos==10)&&(__this->mNeedNewFile == 1))||(__this->mCurRecordPos>__this->mDuration+15))
		{
			
			struct mymesg *mesg=NULL;
			struct mymesg amesg;
			if(__this->mUseMsgQ)
			{
				mesg = &amesg;
			}else
			
			{
				mesg=(struct mymesg *)malloc(sizeof(struct mymesg));
			    if(mesg==NULL)
			    {
			        ALOGE("__timer_cb malloc fail");
			        return ;
			    }
			}
			mesg->mesg_type = RECORD_CMD_TIMEOUT;
			mesg->timestamp = (int64_t)systemTime();
			memset(&mesg->mesg_data[0],0,sizeof(CdxMuxerPacketT));//blank
			mesg->mesg_len = sizeof(CdxMuxerPacketT);

			int pp=0;
			LOGW("record __timer_cb timeout  id=%d pos=%d pp=%p",__this->mCameraId,__this->mCurRecordPos,&pp);
			//fprintf(stderr,"record __timer_cb timeout  id=%d pos=%d pp=%p\r\n",__this->mCameraId,__this->mCurRecordPos,&pp);
			 if(__this->mUseMsgQ)
			 {
				int ret=msgQSend(__this->mQueueRecordFile,(char *)mesg,sizeof(struct mymesg),1);
				 if (ret != 0)
				 {
					 LOGW("qxxxrecord __timxxer_cb timeout  id=%d",__this->mCameraId);
					 fprintf(stderr,"qxxxxrecord __timer_cb timeout	id=%d \r\n",__this->mCameraId);
					 pthread_mutex_lock(&__this->mRecordFileMutex);
					 pthread_cond_signal(&__this->mRecordFileCond);
					 pthread_mutex_unlock(&__this->mRecordFileMutex);

				 }
			 }
			 else
			 {
				int ret = OSAL_QueueUrgent(&__this->mQueueBufferRecordFile, mesg,&pp);
			    if (ret != 0)
			    {
			        ALOGE("xxxrecord __timxxer_cb timeout  id=%d",__this->mCameraId);
			        free(mesg);
			        pthread_mutex_lock(&__this->mRecordFileMutex);
			        pthread_cond_signal(&__this->mRecordFileCond);
			        pthread_mutex_unlock(&__this->mRecordFileMutex);
			        return ;

			    }
				ALOGW("get a  pp = %p",pp);
				struct mymesg *pmg=(struct mymesg *)pp;
				
				if(pmg!=NULL)
				{				
					CdxMuxerPacketT *ppkt=(CdxMuxerPacketT *)&pmg->mesg_data[0];
					ALOGW("ppkt data-----%p------",ppkt);

					if(ppkt!=NULL){
						if(ppkt->buf!=NULL){
							ALOGW("free ppkt->buf = %p",ppkt->buf);
							free(ppkt->buf);
							
						}
					}
					ALOGW("free pmg = %p",pmg);
					free(pmg);
					pmg=0;
				}
			}
			ALOGW("release done-----------");
			
		    pthread_mutex_lock(&__this->mRecordFileMutex);
		    pthread_cond_signal(&__this->mRecordFileCond);
		    pthread_mutex_unlock(&__this->mRecordFileMutex);
			return;
	
		}
			
		if(__this->mCurRecordPos==__this->mDuration-1){
			ALOGW("__timer_cb is %d",__this->mCurRecordPos);
			__this->mNeedNewFile = 1;//temply no mutex pretect 
		}
		
	}
}



int dvr_factory::sendCmd2MiscWork(int cmd,void *data,int len){
	struct mymesg *mesg=(struct mymesg *)malloc(sizeof(struct mymesg));
	  if(mesg==NULL)
	  {
		  LOGW("sendCmd2MiscWork malloc fail");
		  return -1;
	  }
	
	  memcpy(&mesg->mesg_data[0],data,len);//blank

	  mesg->mesg_len = len;
	  mesg->mesg_type = cmd;
	  
	  int ret = OSAL_Queue(&mQueueBufferMiscWork, mesg);
	  if (ret != 0)
	  {
		  ALOGE("miscwork queue full  id=%d",mCameraId);
		  free(mesg);
		  pthread_mutex_lock(&mMiscWorkMutex);
		  pthread_cond_signal(&mMiscWorkCond);
		  pthread_mutex_unlock(&mMiscWorkMutex);
		  return FAILED_TRANSACTION;
	
	  }
	  pthread_mutex_lock(&mMiscWorkMutex);
	  pthread_cond_signal(&mMiscWorkCond);
	  pthread_mutex_unlock(&mMiscWorkMutex);
	  return 0;

}


#if 1
//-------------------------------------------------------------------
 /*
　　位图文件的组成
          结构名称 符 号
	位图文件头 (bitmap-file header) BITMAPFILEHEADER bmfh
	位图信息头 (bitmap-information header) BITMAPINFOHEADER bmih
	彩色表　(color table) RGBQUAD aColors[]
	图象数据阵列字节 BYTE aBitmapBits[]
  */
typedef struct bmp_header
{
	short twobyte           ;//两个字节，用来保证下面成员紧凑排列，这两个字符不能写到文件中
	     //14B
	char bfType[2]          ;//!文件的类型,该值必需是0x4D42，也就是字符'BM'
	unsigned int bfSize     ;//!说明文件的大小，用字节为单位
	unsigned int bfReserved1;//保留，必须设置为0
	unsigned int bfOffBits  ;//!说明从文件头开始到实际的图象数据之间的字节的偏移量，这里为14B+sizeof(BMPINFO)
}BMPHEADER;

typedef struct bmp_info
{
	     //40B
	 unsigned int biSize         ;//!BMPINFO结构所需要的字数
	 int biWidth                 ;//!图象的宽度，以象素为单位
	 int biHeight                ;//!图象的宽度，以象素为单位,如果该值是正数，说明图像是倒向的，如果该值是负数，则是正向的
	 unsigned short biPlanes     ;//!目标设备说明位面数，其值将总是被设为1
	 unsigned short biBitCount   ;//!比特数/象素，其值为1、4、8、16、24、或32
	 unsigned int biCompression  ;//说明图象数据压缩的类型
	 #define BI_RGB        0L    //没有压缩
	 #define BI_RLE8       1L    //每个象素8比特的RLE压缩编码，压缩格式由2字节组成（重复象素计数和颜色索引）；
	 #define BI_RLE4       2L    //每个象素4比特的RLE压缩编码，压缩格式由2字节组成
	 #define BI_BITFIELDS  3L    //每个象素的比特由指定的掩码决定。
	 unsigned int biSizeImage    ;//图象的大小，以字节为单位。当用BI_RGB格式时，可设置为0
	 int biXPelsPerMeter         ;//水平分辨率，用象素/米表示
	 int biYPelsPerMeter         ;//垂直分辨率，用象素/米表示
	 unsigned int biClrUsed      ;//位图实际使用的彩色表中的颜色索引数（设为0的话，则说明使用所有调色板项）
	 unsigned int biClrImportant ;//对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。
}BMPINFO;

typedef struct tagRGBQUAD
{
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO
{
	BMPINFO    bmiHeader;
	//RGBQUAD    bmiColors[1];
	unsigned int rgb[3];
} BITMAPINFO;


static int get_rgb565_header(int w, int h, BMPHEADER * head, BITMAPINFO * info)
{
    int size = 0;
    if (head && info)
    {
        size = w * h * 2;
        memset(head, 0, sizeof(* head));
        memset(info, 0, sizeof(* info));
         head->bfType[0] = 'B';
         head->bfType[1] = 'M';
         head->bfOffBits = 14 + sizeof(* info);
         head->bfSize = head->bfOffBits + size;
         head->bfSize = (head->bfSize + 3) & ~3;
         size = head->bfSize - head->bfOffBits;

         info->bmiHeader.biSize = sizeof(info->bmiHeader);
         info->bmiHeader.biWidth = w;
         info->bmiHeader.biHeight = -h;
         info->bmiHeader.biPlanes = 1;
         info->bmiHeader.biBitCount = 16;
         info->bmiHeader.biCompression = BI_BITFIELDS;
         info->bmiHeader.biSizeImage = size;

         info->rgb[0] = 0xF800;
         info->rgb[1] = 0x07E0;
         info->rgb[2] = 0x001F;

         ALOGD("rgb565:%dbit,%d*%d,%d\n", info->bmiHeader.biBitCount, w, h, head->bfSize);
     }
     return size;
 }


static int save_bmp_rgb565(FILE* fp, int width, int height, unsigned char* pData)
{
	int success = 0;
	int size = 0;
	BMPHEADER head;
	BITMAPINFO info;
	size = get_rgb565_header(width, height, &head, &info);
	if(size > 0)
	{

        fwrite(head.bfType,1,2,fp);
        fwrite(&head.bfSize,1,4,fp);
        fwrite(&head.bfReserved1,1,4,fp);
        fwrite(&head.bfOffBits,1,4,fp);

		fwrite(&info,1,sizeof(info), fp);
		fwrite(pData,1,size, fp);
		success = 1;
	}
	ALOGD("**save_bmp_rgb565***success=%d\n", success);
	return success;
}
#endif
bool dvr_factory::DvrMiscWorkThread()
{
    int ret;
    int need_new=0;
    int qcnt;
    struct mymesg  * pmsg;

    pmsg = (struct mymesg *)OSAL_Dequeue(&mQueueBufferMiscWork);
    if (pmsg == NULL)
    {
     	qcnt = OSAL_GetElemNum(&mQueueBufferMiscWork);
        ALOGV("MiscWork queue no buffer, sleep.cnt=%d..id=%d",qcnt,mCameraId);
        pthread_mutex_lock(&mMiscWorkMutex);
        pthread_cond_wait(&mMiscWorkCond, &mMiscWorkMutex);
        pthread_mutex_unlock(&mMiscWorkMutex);

        return true;
    }
	if(pmsg->mesg_type == MISCWORK_CMD_RM)
	{
		ALOGI("DvrMiscWorkThread rm file (%s).\n",pmsg->mesg_data);
		remove(pmsg->mesg_data);
		//memset(pmsg->mesg_data,0,CM_MAX_FILE_LEN);
		free(pmsg);
		ALOGI("DvrMiscWorkThread rm file done");
	}
		if(pmsg->mesg_type == MISCWORK_CMD_SAVETHUMB)
	{
		ALOGI("DvrMiscWorkThread MISCWORK_CMD_SAVETHUMB file (%s).\n",pmsg->mesg_data);
		int fd=generateThumbFile();
				ALOGV("store pic us time us fd=%d",fd);
		miscmsg_t *pmmsg=	(miscmsg_t  *)&pmsg->mesg_data[0];	

		 struct tm *tm=NULL;
		 file_status_t *p =&mFilemanger;
		 char buf[128]= {0};
		 buf[128]= 0;
		
		 sprintf(buf, "%s%s%s%d%c%s%s",p->cur_filedir[0],"/",CM_THUMB_DIR,p->cur_file_idx,'-',"thumb.","bmp");
		 FILE *fp = fopen(buf, "wb");
		 save_bmp_rgb565(fp,pmmsg->w,pmmsg->h,(unsigned char*)pmmsg->msgdata);

		//fsync(fp);
		fclose(fp);
		free(pmmsg->msgdata);
		free(pmsg);
		ALOGI("DvrMiscWorkThread MISCWORK_CMD_SAVETHUMB file done");
	}
	
	return true;


}


