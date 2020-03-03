//SDKLIB_TEST have already defined by Makefile
#ifdef SDKLIB_TEST
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/fb.h>

#include "hwdisp2.h"
#include "aw_ini_parser.h"
#include <sdklog.h>
#include "CameraDebug.h"
#include "hwdisp2.h"

#include "V4L2CameraDevice2.h"
#include "CallbackNotifier.h"
#include "PreviewWindow.h"
#include "CameraHardware2.h"
#include "HALCameraFactory.h"
#include "CameraHardwareInterface.h"
#include "audio_hal.h"
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
#include <time.h>

#include "Rtc.h"
#include "StorageManager.h"
#include "DvrFactory.h"
#include "CameraFileCfg.h"
#include <sys/socket.h>
#include <sys/un.h>
#include "Fat.h"
#include "DebugLevelThread.h"
#include "DvrRecordManager.h"
using namespace android;
int g_recordstatus=0;
//#define DB_LEVEL_TEST
//#define FORMATE_TEST
//#define RECORD_TEST 1
//#define ADB_SERVICE
#define AUDIO_TEST
//#define ROTATE_TEST
//#define ANOTHER_CAMERA_TEST 1
#define AUTEVENT_TEST 1   /*sdcard/usb auto detect*/


//#define DISPLAY_A_YUV_FILE 
//#define SMART_REMOVE_TEST 1   //test 文件统一管理，创建和删除
#ifdef AUTEVENT_TEST
#include "NetlinkManager.h"
#include "NetlinkHandler.h"
#include "NetlinkEvent.h"
#endif

//for some gpu user for texture import ,dont define this in this file,define at makefile_sub
// DMAFD_TEST

#ifdef DISPLAY_A_YUV_FILE
	int video_layer0;
	void ShowYUVpictureDemo(HwDisplay* mcd);
#endif

#define TEST_CAMERA_ID 0
#define TEST_CAMERA_ID_T1 0
#define TEST_CAMERA_ID_T2 1

#define PATH_SDCARD  "/mnt/sdcard/mmcblk1p1/"
#define PATH_SD_DEV  "/dev/mmcblk1p1"
#ifdef USE_FAT_TEST
extern "C"  void *fattestmain(void *arg);

#endif
extern "C" int aut_adb_main(int argc, char **argv);

dvr_factory *pdvr=NULL;

dvr_factory *pdvr4=NULL;
dvr_factory *pdvr5=NULL;
dvr_factory *pdvr6=NULL;
dvr_factory *pdvr7=NULL;

#ifdef ANOTHER_CAMERA_TEST	
	dvr_factory *pdvr1=NULL;
#endif
status_t usr_datacb(int32_t msgType, char *dataPtr,int dalen, void *user){
    return 0;
}

void usernotifyCallback(int32_t msgType, int32_t ext1, int32_t ext2, void* user){
	LOGE("msgType =%d-----data=%p-----%d)",msgType,user);

    if ((msgType&CAMERA_MSG_ERROR) ==CAMERA_MSG_ERROR)
    {
        LOGE("(msgType =CAMERA_MSG_ERROR)");

    }
    if ((msgType&CAMERA_MSG_DVR_NEW_FILE) ==CAMERA_MSG_DVR_NEW_FILE)
    {
        dvr_factory *p_dvr=(dvr_factory *)user;
        LOGE("(msgType =CAMERA_MSG_DVR_NEW_FILE camera %d idx =%d)",p_dvr->mCameraId,ext1);
    }

    if ((msgType&CAMERA_MSG_DVR_STORE_ERR) ==CAMERA_MSG_DVR_STORE_ERR)
    {
        LOGE("msgType =CAMERA_MSG_DVR_STORE_ERR)");
		dvr_factory *p_dvr=(dvr_factory *)user;
		p_dvr->storage_state=0;//tmp
		p_dvr->stopRecord();
    }
}

void userdataCallback(int32_t msgType,char* dataPtr, camera_frame_metadata_t *metadata, void* user){
	//LOGE("userdataCallback=-----------)");
}

void userdataCallbackTimestamp(nsecs_t timestamp, int32_t msgType, char* dataPtr, void* user){
	char *ptp=(char *)dataPtr;
	VencInputBuffer *p=(VencInputBuffer *)(ptp+4);
	// memcpy(inputBuffer.pAddrVirY,(unsigned char*)p->pAddrVirY,encode_param.src_width*encode_param.src_height);
	// memcpy(inputBuffer.pAddrVirC,(unsigned char*)p->pAddrVirY+encode_param.src_width*encode_param.src_height,encode_param.src_width*encode_param.src_height/2);
}

#ifdef ADAS_ENABLE
void userAdasDataCallback(int32_t msgType, char *dataPtr, void *metadata, void* user){
	if(CAMERA_MSG_ADAS_METADATA == msgType){
		ADASOUT_IF * padasResult=(ADASOUT_IF *)metadata;
		//LOGV("userAdasDataCallback ,hi i get some adas result vanishLine =%f",padasResult->ldw_result.autoCalVanishLine);
		//padasResult->ldw_result
		//padasResult->fcw_result
	}
}
#endif

#ifdef AUTEVENT_TEST
void testaut_event_cb_func(NetlinkEvent *evt,void *usrdata){
	ALOGV("event act=%d %s",evt->getAction(),evt->getSubsystem());
	
    int action = evt->getAction();
    const char *devtype = evt->findParam("DEVTYPE");

    if (action == NetlinkEvent::NlActionAdd)
    {
        int major = atoi(evt->findParam("MAJOR"));
        int minor = atoi(evt->findParam("MINOR"));
        char nodepath[255];

        if (!strcmp(devtype, "disk")){
			ALOGW("line=%d,devtype=%s",__LINE__,devtype);
			//handleDiskAdded(dp, evt);
			char bufname[512];
			memset(bufname,0,512);
			char tmpbuf[256];
			char real_path[256];
			config_get_curfiledir(0,bufname);
			int len = strlen(bufname);
			const char *tmp = evt->findParam("NPARTS");
			if (tmp) {		  
			int npart = atoi(tmp); 
			if(npart==0)
			{
				const char *devname = evt->findParam("DEVNAME");
				ALOGW("devname=%s",devname);
				char buf[256];
				if(strstr(devname,"sd")){
					sprintf(buf,"mount -t vfat /dev/%s /mnt/usb -o defaults,noatime,async,iocharset=cp936",devname);
				}else if(strstr(devname,"mmcblk1")){
					sprintf(buf,"fsck_msdos -p -f /dev/%s",devname);
					system(buf);
					sprintf(buf,"mount -t vfat /dev/%s /mnt/sdcard/mmcblk1p1 -o defaults,noatime,async,iocharset=cp936",devname);
					system("mkdir -p /mnt/sdcard/mmcblk1p1");
					system(buf);
				}else{
					sprintf(buf,"echo mount fail > /dev/ttyS0");
				}
			}
		} else {		
			SLOGW("Kernel block uevent missing 'NPARTS'");
		}
		strcpy(tmpbuf,bufname);
		for(int i=0; i<len; i++){
			if(tmpbuf[len-1-i]=='/'){
				tmpbuf[len-1-i]=0;
			}
			else{
				break;
			}
		}
		
		int rmret = readlink(tmpbuf,real_path,sizeof(real_path));
		if(rmret<0){
			ALOGV("mount path not a symlink %s",tmpbuf);
			strcpy(real_path,tmpbuf);
		}
		else{
			ALOGV("mount real path is %s \r\n",real_path);
		}
		
		for(int i=0;i<6;i++){
			if(isMounted(real_path))
				break;
			sleep(1);
		}
		
		if(g_recordstatus==0){
			pdvr->startRecord();
			
			#ifdef ANOTHER_CAMERA_TEST	
				pdvr1->startRecord();
			#endif
			
			g_recordstatus = 1;
		}else
			ALOGE("record status error g_recordstatus=%d",g_recordstatus);
		}
    }
    else if (action == NetlinkEvent::NlActionRemove)
    {
        if (!strcmp(devtype, "disk"))
        {
        	if(g_recordstatus==1){
				pdvr->storage_state=0;
				pdvr->stopRecord();
				g_recordstatus = 0;
				ALOGE("sdcard has remove,stopRecord now,g_recordstatus=%d",g_recordstatus);
				
				#ifdef ANOTHER_CAMERA_TEST	
					pdvr1->storage_state=0;
					pdvr1->stopRecord();
				#endif
        	}
		}
    }


}
#endif

#ifdef DMAFD_TEST
	extern "C" int create_textture();
	extern "C" int info_aframe(int* dmafd);
#endif

void testStop(int signo)
{
    printf("oops! stop!!! %d\n",signo);
    _exit(0);
}

#ifdef DISP_LIB_V20
static void TvOutNtsc(HwDisplay* mcd,int screen_id,unsigned int *hlay,struct view_info *surface,int mode)
{
	if((NULL == mcd)||(NULL == hlay)){
		printf("%s input arg error\r\n",__FUNCTION__);
	}
	
	printf("test_layer%d_tvout_ntsc begin\r\n",screen_id);
	mcd->aut_hwd_layer_sufaceview(*hlay,surface);
	mcd->hwd_other_screen(screen_id,DISP_OUTPUT_TYPE_TV,mode);
}

static void HdmiOut(HwDisplay* mcd,int screen_id,unsigned int *hlay,struct view_info *surface,int mode){
	if((NULL == mcd)||(NULL == hlay)){
		printf("%s input arg error\r\n",__FUNCTION__);
	}
	
	mcd->aut_hwd_layer_sufaceview(*hlay,surface);
	mcd->aut_hwd_layer_set_rect(*hlay,surface);
	mcd->hwd_other_screen(screen_id,DISP_OUTPUT_TYPE_HDMI,mode); 
}

static void VgaOut(HwDisplay* mcd,int screen_id,unsigned int *hlay,struct view_info *surface,int mode){
	if((NULL == mcd)||(NULL == hlay)){
		printf("%s input arg error\r\n",__FUNCTION__);
	}
	
	printf("test_layer%d_vga %d*%d\r\n",screen_id,surface->w,surface->h);
	mcd->aut_hwd_layer_sufaceview(*hlay,surface);
	mcd->aut_hwd_layer_set_rect(*hlay,surface);
	mcd->hwd_other_screen(screen_id,DISP_OUTPUT_TYPE_VGA,mode ); 
}
#endif


int main(int argc,char *argv[])
{
	signal(SIGINT, testStop);
	int cvbscameartest=0;
	int test1=0;
	int test2=0;
#ifdef AUTEVENT_TEST
	NetlinkManager *nm;
    if (!(nm = NetlinkManager::Instance())) {
        SLOGE("Unable to create NetlinkManager");
    };

	AutEvent *pevent=AutEvent::Instance();

	//
	pevent->setEventCb(testaut_event_cb_func,(void *)NULL);


	if(cvbscameartest == 1){
		pevent->setEventCb(testaut_event_cb_func,(void *)NULL);
		pevent->setEventCb(testaut_event_cb_func,(void *)NULL);
		pevent->setEventCb(testaut_event_cb_func,(void *)NULL);
		pevent->setEventCb(testaut_event_cb_func,(void *)NULL);
	}
    if (nm->start()) {
        SLOGE("Unable to start NetlinkManager (%s)", strerror(errno));
    }
#endif

#ifdef DB_LEVEL_TEST
	DebugLevelThread * pdebuglevel = new DebugLevelThread();
#endif

#ifdef FORMATE_TEST
    Fat  *pfat = new Fat();
    if(NULL == pfat){
		printf("new Fat() error\n");
    }else{
    	pfat->format(PATH_SD_DEV, PATH_SDCARD);
    }
#endif

#ifdef DISPLAY_A_YUV_FILE
	HwDisplay* mcd=HwDisplay::getInstance();
	if(NULL == mcd){
		printf("get HwDisplay instance error,return now\n");
		return -1;
	}
	ShowYUVpictureDemo(mcd);
	sleep(1000);
#endif	

#ifdef DMAFD_TEST
	create_textture();
#endif

#ifdef USE_FAT_TEST
	pthread_t write_50_file_pthread;

	int res=pthread_create(&write_50_file_pthread, NULL, fattestmain,NULL);
	if(res !=0){
		fprintf(stderr, "create fattestmain failed, please try again!\n");
	}
#endif



	int flag360=0;
	char *name;
	
	sdk_log_setlevel(6);
	
	printf("--------------argc =%d \r\n",argc);
	if(argc >1){
		if(strstr(argv[1],"360")){
			flag360=1;
		}if(strstr(argv[1],"cvbs")){
			cvbscameartest = 1;//not devp compete todo
		}
		else {
			name=argv[1];
		}
		printf("------------argv =%s \r\n",argv[1]);
	}else{
		name="0";
	}
	
	HwDisplay* mcd=HwDisplay::getInstance();
	if(NULL == mcd){
		printf("get HwDisplay instance error,return now\n");
		return -1;
	}
//renewcamera:	
	if(flag360)
		pgEmulatedCameraFactory=new HALCameraFactory(1);
	else
		pgEmulatedCameraFactory=new HALCameraFactory();
		
#ifdef SMART_REMOVE_TEST
	#ifdef SMART_RECORD
	DvrRecordManagerInit();  
	#endif
#endif	
#ifdef AUDIO_TEST
	//AudioCapThread audiocap;
	sp<AudioCapThread> audiocap=new AudioCapThread();
#endif

renewdvr:
	
	pdvr=new dvr_factory();
	if(cvbscameartest == 1){
		pdvr4=new dvr_factory();
		pdvr5=new dvr_factory();
		pdvr6=new dvr_factory();
		pdvr7=new dvr_factory();
	}
	#if 0
	//test a bug
	AudioEncConfig mAudioEncConfig;
	
	memset(&mAudioEncConfig, 0x00, sizeof(AudioEncConfig));
	mAudioEncConfig.nInChan = 2;
	mAudioEncConfig.nInSamplerate = 44100;
	mAudioEncConfig.nOutChan = 2;
	mAudioEncConfig.nOutSamplerate = 44100;
	mAudioEncConfig.nSamplerBits = 16;
	mAudioEncConfig.nType = AUDIO_ENCODER_AAC_TYPE;
	mAudioEncConfig.nFrameStyle = 1;
	AudioEncoder*  pAudioEncode = CreateAudioEncoder();

    if(!pAudioEncode)
    {
        ALOGE("AudioEncInit failed");
        return -1;
    }

    if(InitializeAudioEncoder(pAudioEncode, &mAudioEncConfig) < 0)
    {
        ALOGE("AudioEncInit init failed");
        return -1;
    }
	if(pAudioEncode)
    {
        DestroyAudioEncoder(pAudioEncode);
        pAudioEncode=NULL;
    }
	#endif
#ifdef AUDIO_TEST
	//AudioCapThread audiocap;
	//sp<AudioCapThread> audiocap=new AudioCapThread();
	int hdl=audiocap->reqAudioSource(pdvr->__audio_data_cb_timestamp,(void *) pdvr);
	pdvr->setAudioOps(audiocap,hdl);
	if(cvbscameartest == 1){

		pdvr4->setAudioOps(audiocap,hdl);
		pdvr5->setAudioOps(audiocap,hdl);
		pdvr6->setAudioOps(audiocap,hdl);
		pdvr7->setAudioOps(audiocap,hdl);
	}
	
#endif

#ifdef DISP_LIB_V20
	int screen_id=0;
	int ch_id = 0;
	int lyr_id = 0;
	int Zorder = 5;
	struct view_info surface={0,0,1024,600};

	surface.w = 1024;
	surface.h = 600;
	int video_layer0=mcd->aut_hwd_layer_request(&surface,screen_id,ch_id,lyr_id);

	mcd->aut_hwd_layer_set_zorder(video_layer0,Zorder);
	mcd->aut_hwd_layer_sufaceview(video_layer0,&surface);
	mcd->aut_hwd_layer_set_rect(video_layer0,&surface);

	screen_id = 1;
	ch_id = 0;
	lyr_id = 0;
	Zorder = 2;
	int video_layer1=mcd->aut_hwd_layer_request(&surface,screen_id,ch_id,lyr_id);
	mcd->aut_hwd_layer_set_zorder(video_layer1,Zorder);
	
	//----------------------TV OUT CFG-------------------------------
	surface.w = 720;
	surface.h = 480;
	TvOutNtsc(mcd,screen_id,(unsigned int *)&video_layer1,&surface,DISP_TV_MOD_NTSC);


	//----------------------VGA OUT CFG------------------------------
	//surface.w = 800;
	//surface.h = 600;
	//VgaOut(mcd,screen_id,(unsigned int *)&video_layer1,&surface,DISP_VGA_MOD_800_600P_60);

	
	//----------------------HDMI OUT CFG-----------------------------
	//surface.w = 1920;
	//surface.h = 1080;
	//HdmiOut(mcd,screen_id,(unsigned int *)&video_layer1,&surface,DISP_TV_MOD_1080P_25HZ);	

	/*
	DISP_TV_MOD_720P_60HZ           = 5,    ok
	DISP_TV_MOD_1080I_50HZ          = 6, 	ok
	DISP_TV_MOD_1080I_60HZ          = 7,	ok
	DISP_TV_MOD_1080P_24HZ          = 8,  	ok
	DISP_TV_MOD_1080P_50HZ          = 9,	ok
	DISP_TV_MOD_1080P_60HZ          = 0xa,	ok
	DISP_TV_MOD_1080P_24HZ_3D_FP    = 0x17,
	DISP_TV_MOD_720P_50HZ_3D_FP     = 0x18,
	DISP_TV_MOD_720P_60HZ_3D_FP     = 0x19,
	DISP_TV_MOD_1080P_25HZ          = 0x1a,	ok
	DISP_TV_MOD_1080P_30HZ          = 0x1b,
	DISP_TV_MOD_PAL                 = 0xb, 	ok
	DISP_TV_MOD_PAL_SVIDEO          = 0xc,
	DISP_TV_MOD_NTSC                = 0xe,
	DISP_TV_MOD_NTSC_SVIDEO         = 0xf,
	DISP_TV_MOD_PAL_M               = 0x11,
	DISP_TV_MOD_PAL_M_SVIDEO        = 0x12,
	DISP_TV_MOD_PAL_NC              = 0x14,
	DISP_TV_MOD_PAL_NC_SVIDEO       = 0x15,
	DISP_TV_MOD_3840_2160P_30HZ     = 0x1c,  not support
*/

	
#else
	int tvmode=config_get_tvout(TEST_CAMERA_ID);
	printf("--------------tvmode =%d \r\n",tvmode);
	//tvmode:0x0b~pal_TV  0x0e~ntsc_TV   0x05~720pHDMI  0x09~1080pHDMI  0x51~800*600VGA
	//mcd->hwd_screen1_mode(tvmode);//for dual disp at the same time
	//mcd->hwd_screen1_mode(DISP_VGA_MOD_800_600P_60);
	mcd->hwd_screen1_mode(DISP_TV_MOD_1080P_50HZ);
	//mcd->hwd_tvout();//tv ntsc
	//mcd->hwd_hdmiout();//720p
	//mcd->hwd_other_screen(0,DISP_OUTPUT_TYPE_HDMI,DISP_TV_MOD_1080P_60HZ);//1080p
	//mcd->hwd_other_screen(0,DISP_OUTPUT_TYPE_TV,DISP_TV_MOD_PAL);
#endif

	int i=1;
    while(i>0)
    {	
		i--;
		if(flag360){
			printf("is 360 camera \r\n",argv[1]);
			#ifdef DEFAULT_CVBS_CAMERA_TYPE_PAL
				config_set_heigth(360,1152);//1152 pal
			#else
				config_set_heigth(360,960);//960 ntsc
			#endif
			config_set_weith(360,1440);
			name = "360";
		}

		int rt = 0;
		#ifdef RECORD_ALONE
			rt = pdvr->initializeDev(name);
			if(0 != rt){
				ALOGE("pdvr->initializeDev error");
			}
		    pdvr->recordwith  = config_get_weith(pdvr->mCameraId);
	    	pdvr->recordheith = config_get_heigth(pdvr->mCameraId);
		#else

		rt= pdvr->recordInit("0");
		if(cvbscameartest == 1){
			rt= pdvr4->recordInit("4");
			rt= pdvr5->recordInit("5");
			rt= pdvr6->recordInit("6");
			rt= pdvr7->recordInit("7");
		}
		#endif

        pdvr->SetDataCB(usr_datacb,pdvr);
        pdvr->setCallbacks(usernotifyCallback,userdataCallback,userdataCallbackTimestamp,pdvr);
		if(cvbscameartest == 1){

			pdvr4->SetDataCB(usr_datacb,pdvr4);
	        pdvr4->setCallbacks(usernotifyCallback,userdataCallback,userdataCallbackTimestamp,pdvr4);
	        pdvr5->SetDataCB(usr_datacb,pdvr5);
	        pdvr5->setCallbacks(usernotifyCallback,userdataCallback,userdataCallbackTimestamp,pdvr5);
	        pdvr6->SetDataCB(usr_datacb,pdvr6);
	        pdvr6->setCallbacks(usernotifyCallback,userdataCallback,userdataCallbackTimestamp,pdvr6);
	        pdvr7->SetDataCB(usr_datacb,pdvr7);
	        pdvr7->setCallbacks(usernotifyCallback,userdataCallback,userdataCallbackTimestamp,pdvr7);
		}
        #if WHITEBELANCE_TEST
	        ALOGV("setWhiteBlanceData begine");
			pdvr->setWhiteBlanceData(16);
			
			ALOGV("setColorEffectData begine");
			pdvr->setColorEffectData(17);
			
			ALOGV("setExposureCompensationData begine");
			pdvr->setExposureCompensationData(18);
		#endif
		
        pdvr->start();
		if(cvbscameartest == 1){

	        pdvr4->start();
	        pdvr5->start();
	        pdvr6->start();
	        pdvr7->start();
		}
	#ifndef SONIX_USB
		char bufname[512];
        pdvr->enableWaterMark();
        sprintf(bufname,"64,64,0,64,250,T3L SDK,64,450,ALLWINNER");//watermark only support upper case string
        pdvr->setWaterMarkMultiple(bufname);
	#endif

		struct view_info vv= {0,0,1024,600};
		ALOGV("vx=%d vy=%d sx=%d sy%d",mcd->lcdxres,mcd->lcdyres,pdvr->recordwith,pdvr->recordheith);
		//pdvr->startPriview(vv);

	#ifdef RECORD_ALONE
		int retrec=pdvr->recordInit(name);
	#endif
		int retrec=pdvr->startRecord();


	#ifdef ANOTHER_CAMERA_TEST
		pdvr1=new dvr_factory();
		int hdl2=audiocap->reqAudioSource(pdvr1->__audio_data_cb_timestamp,(void *) pdvr1);
		pdvr1->setAudioOps(audiocap,hdl2);
	
		#ifdef RECORD_ALONE
			rt = pdvr1->initializeDev("1");
			if(0 != rt){
				ALOGE("pdvr1->initializeDev 1 error");
			}
			pdvr1->recordwith  = config_get_weith(pdvr1->mCameraId);
	    	pdvr1->recordheith = config_get_heigth(pdvr1->mCameraId);
		#else
			rt= pdvr1->recordInit("1");
		#endif

		if(flag360){
			sleep(5);
			config_set_heigth(TEST_CAMERA_ID,720);
			config_set_weith(TEST_CAMERA_ID,1280);
			#ifdef RECORD_ALONE
				//rt= pdvr1->recordInit("0");
			#else
				rt= pdvr1->recordInit("0");
			#endif
			F_LOG;
			sleep(10);
		}
		#ifdef ROTATE_TEST
			aut_set_disp_rotate_mode(TR_ROT_270);//temp call here ,must after all dvr factory
		#endif	
        pdvr1->SetDataCB(usr_datacb,pdvr1);
        pdvr1->setCallbacks(usernotifyCallback,userdataCallback,userdataCallbackTimestamp,pdvr1);
        pdvr1->start();

        #ifdef ADAS_ENABLE	
			pdvr1->setAdasCallbacks(userAdasDataCallback);
			pdvr1->enableAdas();	
		#endif

		#ifdef RECORD_ALONE
			retrec+=pdvr1->recordInit("1");
		#endif
		retrec+=pdvr1->startRecord();
	#endif
	

		if(retrec<0){
			ALOGE("!!start record fail pls insert tf card");
			g_recordstatus=0;
		}else{
			g_recordstatus=1;
		}
	
		ALOGV("------dbg-rec start 1min Record now");
		
	#ifdef ADAS_ENABLE
	#ifdef ANOTHER_CAMERA_TEST
		pdvr1->disableAdas();
	#endif
	#endif

	int preview_cnt=1;
	while(preview_cnt--){
		vv.x = 0;
		vv.y = 0;
		vv.w=1024/2;
		vv.h=600/2;
		printf("strat priview 0 \n");
 		pdvr->startPriview(vv);
		sleep(3);
		pdvr->stopPriview();
		//sleep(3);
		if(cvbscameartest == 1){
		vv.x = 512;
		vv.y = 0;
		vv.w=512;
		vv.h=300;
		printf("strat priview 4 \n");
		pdvr4->startPriview(vv);
		sleep(3);
		pdvr4->stopPriview();
		sleep(3);

		vv.x = 0;
		vv.y = 300;
		vv.w=512;
		vv.h=300;
		printf("strat priview 5 \n");
		pdvr5->startPriview(vv);
		sleep(3);
		pdvr5->stopPriview();
		sleep(3);
		
		vv.x = 512;
		vv.y = 300;
		vv.w=512;
		vv.h=300;
		printf("strat priview 6 \n");
		pdvr6->startPriview(vv);
		sleep(3);
		pdvr6->stopPriview();
		sleep(3);

		vv.x = 0;
		vv.y = 0;
		vv.w=512;
		vv.h=300;
		printf("strat priview 7 \n");
		pdvr7->startPriview(vv);
		sleep(3);
		pdvr7->stopPriview();
		sleep(3);
			}
	}
	
	sleep(1);   
	ALOGV("------dbg-rec stop 1min record now");
	/*pdvr->stopRecord();
	pdvr1->stopRecord();
	pdvr1->stopPriview();
	printf("start record now\n");
	pdvr1->startRecord();
	pdvr->startRecord();
	while(1){
		printf("un mount now\n");
		system("umount -f /dev/mmcblk1");
		sleep(15);
		system("fsck_msdos -p -f /dev/mmcblk1");
		system("mount /dev/mmcblk1 /mnt/sdcard/mmcblk1p1/");
		printf("mount sleep(%d)\n",rand()%30 + 20);
		sleep(rand()%30 + 20);	
	}

	#ifdef RECORD_TEST
		sleep(30*60);   //   30min
		ALOGV("------dbg-rec stop 1min record now");
		pdvr->stopRecord();
		#ifdef ANOTHER_CAMERA_TEST
		pdvr1->stopRecord();
		pdvr1->stopPriview();
		#endif	
		sleep(2);
		pdvr1->setDuration(3*60);
		pdvr1->startRecord();
		pdvr->setDuration(3*60);
		pdvr->startRecord();
		ALOGV("-------------------------------------------dbg-rec start 3min Record now");
		sleep(8*60*3+10);
	#endif
	*/
//}
	pdvr->stopRecord();
	#ifdef AUDIO_TEST
	    audiocap->stopCapture(hdl);
		audiocap->releaseAudioSource(hdl);
	#endif

		F_LOG;
		pdvr->stop();
		if(cvbscameartest == 1){

			pdvr4->stop();
			pdvr5->stop();
			pdvr6->stop();
			pdvr7->stop();
		}
		#ifdef ANOTHER_CAMERA_TEST
			F_LOG;
			pdvr1->stop();
		#endif
		sleep(1);	
		F_LOG;
		pdvr->enc_de_init();
		if(cvbscameartest == 1){
			pdvr4->enc_de_init();
			pdvr5->enc_de_init();
			pdvr6->enc_de_init();
			pdvr7->enc_de_init();
		}
		F_LOG;
		
		pdvr->uninitializeDev();
		if(cvbscameartest == 1){
			pdvr4->uninitializeDev();
			pdvr5->uninitializeDev();
			pdvr6->uninitializeDev();
			pdvr7->uninitializeDev();
		}
		F_LOG;
	
	#ifdef ANOTHER_CAMERA_TEST
		F_LOG;
		pdvr1->enc_de_init();
		F_LOG;
		pdvr1->uninitializeDev();
		F_LOG;
	#endif
	
		sleep(2); //now this needed fix next version
	//pthread_create();
    }   //while end
	
	delete pdvr;
	if(cvbscameartest == 1){
		delete pdvr4;
		delete pdvr5;
		delete pdvr6;
		delete pdvr7;
	}
	if(test1 <2){
		test1++;
		goto renewdvr;
	}

#ifdef FORMATE_TEST
    delete pfat;
    pfat = NULL;
#endif

#ifdef DB_LEVEL_TEST
    delete pdebuglevel;
    pdebuglevel = NULL;
#endif

#ifdef ANOTHER_CAMERA_TEST
	delete pdvr1;
#endif

#ifdef AUDIO_TEST
	audiocap->stopAudio();
#endif

	delete pgEmulatedCameraFactory;
//	if(test2 <100){
//		test2++;
//		goto renewcamera;
//	}
    return 0;
}



#ifdef DISPLAY_A_YUV_FILE
#define PIC_W 736
#define PIC_H 1280
#define PIC_NAME "pic_736x1280_2.yuv"
void ShowYUVpictureDemo(HwDisplay* mcd)
{
	ALOGD("DISPLAY_A_YUV_FILE begin \r\n");
	struct ScMemOpsS* dispmemops = MemAdapterGetOpsS();	 
	
	int ret = CdcMemOpen(dispmemops);

	if (ret < 0)
	{
		ALOGE("ion_alloc_open failed");
		return ;
	}
	int disp_vaddr= (int)CdcMemPalloc(dispmemops, (PIC_W*PIC_H*4+0xfff) & ~0xfff);
	int disp_paddr=(int)CdcMemGetPhysicAddressCpu(dispmemops,(void*)disp_vaddr);
	char  filename[64];
	//sprintf(filename,"/mnt/sdcard/mmcblk1p1/%s",PIC_NAME);
	sprintf(filename,"/tmp/%s",PIC_NAME);
	
	FILE *fpff = fopen(filename,"rb");
	if(NULL == fpff){
		fpff = fopen(filename,"rb");
		if(NULL == fpff){
			printf("fopen %s ERR \r\n",filename);
		}
		else{
			printf("fopen %s OK \r\n",filename);
			fread((void *)disp_vaddr, 1, PIC_W*PIC_H*3/2, fpff);
			fclose(fpff);
		}
	}
	else{
		printf("fopen %s OK \r\n",filename);
		fread((void *)disp_vaddr, 1, PIC_W*PIC_H*3/2, fpff);
		fclose(fpff);
	}
	
	struct view_info surface0={0,0,1024,600};
	struct src_info src={PIC_W,PIC_H,DISP_FORMAT_YUV420_SP_VUVU};
	ALOGD("layer_request  layer0 \r\n");
	video_layer0=mcd->aut_hwd_layer_request(&surface0,0,0,0);
	
	mcd->aut_hwd_layer_set_src(video_layer0,&src,(int)&disp_paddr);
	mcd->aut_hwd_layer_set_zorder(video_layer0,4);

	mcd->aut_hwd_layer_set_alpha(video_layer0,1,0xff);
	
	mcd->aut_hwd_layer_sufaceview(video_layer0,&surface0);
	
	mcd->aut_hwd_layer_open(video_layer0);
	
	mcd->aut_hwd_layer_render(video_layer0, (void *)&disp_paddr,0);
	ALOGD("aut_hwd_layer_render video_layer0");
	sleep(3000);
	CdcMemPfree(dispmemops, (void *)disp_vaddr);
	CdcMemClose(dispmemops);
	while(1);

}
#endif

#endif


