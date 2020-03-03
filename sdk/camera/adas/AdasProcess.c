//fengyun add for adas starting
//#define LOG_NDEBUG 0
#define LOG_TAG "AdasProcess"
#include <cutils/log.h>
#include "AdasProcess.h"
#include <time.h>
#include <string.h>

#define LOGD(...) //__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
#define LOGE(...) //__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);


int PREVIEW_WIDTH  = 1280;
int PREVIEW_HEIGHT = 720;
int CROPPING_X = 475;
int CROPPING_Y = 199;
int CROPPING_WIDTH = 970;
int CROPPING_HEIGHT = 682;
int ADAS_WIDTH = 970;
int ADAS_HEIGHT = 682;
float SCALE_RATIO = 1.0;

sResultLDW g_ldw_result;
sResultFCW g_fcw_result;
int mdas_major;
int mdas_minor;
int mdas_build;

int g_adas_init = 0;

//just for settings parameters start
double g_camera_sensor_width = 3.0;
double g_camera_sensor_height = 3.0;
double g_camera_sensor_scale = 1.0;
int g_i_format = IF_YUV420_NV21;
double g_dDownscale = 1.0;
double g_lens_efl = 3.2;
int    g_algo_vanish_line=539;
int    g_can_data=100;

int adas_value[ADAS_TOTAL_NUM] = 
{100,//ADAS_CAN_DATA_SPEED
	1280,//WIDTH
	720,//HEIGHT
	1024,	//ADAS_APP_DISP_WIDTH,
	469,//ADAS_APP_DISP_HEIGHT,
	25,//ADAS_CAMERA_FRAME
	130,//ADAS_CAMERA_HEIGHT
	6,//ADAS_TTC
	540,//ADAS_VANISH_LINE
	0,//ADAS_CAMERA_CENTER_OFFSET
	180,//ADAS_CAR_WIDTH
	100,//ADAS_CAMERA_2_WHEEL
	150,//ADAS_CAM_2_BUMPER
	0,//ADAS_LDW_SENSITY_LEFT
	0,//ADAS_LDW_SENSITY_RIGHT
	800,//ADAS_HOOD_LINE
	300,//ADAS_DISTANCE cm
	40,//ADAS_LDW_VELOCITY
	30,//ADAS_FCW_ACTIVITY_VELOCITY
	420, //ADAS_VEHICLE_CENTER_LINE
};

int adas_value_cur[ADAS_TOTAL_NUM] = 
{100,//ADAS_CAN_DATA_SPEED
	1280,//WIDTH
	720,//HEIGHT
	1024,	//ADAS_APP_DISP_WIDTH,
	469,//ADAS_APP_DISP_HEIGHT,
	25,//ADAS_CAMERA_FRAME
	130,//ADAS_CAMERA_HEIGHT
	6,//ADAS_TTC
	540,//ADAS_VANISH_LINE
	0,//ADAS_CAMERA_CENTER_OFFSET
	180,//ADAS_CAR_WIDTH
	100,//ADAS_CAMERA_2_WHEEL
	150,//ADAS_CAM_2_BUMPER
	0,//ADAS_LDW_SENSITY_LEFT
	0,//ADAS_LDW_SENSITY_RIGHT
	800,//ADAS_HOOD_LINE
	300,//ADAS_DISTANCE cm
	40,//ADAS_LDW_VELOCITY
	30,//ADAS_FCW_ACTIVITY_VELOCITY
	420, //ADAS_VEHICLE_CENTER_LINE
};

int fcw_setAdasParameters(int key,int value)
{
	if((key>=0) && (key < ADAS_TOTAL_NUM))
	{
		if ( key == ADAS_CAN_DATA_SPEED ) g_can_data = value;
		else if ( key == ADAS_VANISH_LINE ) adas_value[key] = (value > CROPPING_Y)? ((float)(value - CROPPING_Y)/SCALE_RATIO) : 0;
		else if ( key == ADAS_VEHICLE_CENTER_LINE ) {
			adas_value[key] = (value > CROPPING_X)? ((float)(value - CROPPING_X)/SCALE_RATIO) : 0;
			LOGE("VEHICLE CENTER LINE : %d\n", adas_value[key]);
		}
		else adas_value[key] = value;
	}
	else
	{
		ALOGE("set key=%d error!",key);
	}
	return 0;
}

void fcw_close (void)
{
        while(M_IsProcessing() )
                usleep(1000);
        M_Destroy();
        g_adas_init = 0;
}

static float fcw_get_camera_angle ( int vanish_line )
{
	return M_GetCalib_CalcTiltedAngle(vanish_line);
}

static void fcw_init_algo (float camera_angle )
{   
	int hood_line;
	char *version;
	char *next_tok;
		
    if( !M_Initialize( ALG_LDW | ALG_FCW,0 ) )
    {
       unsigned int    uiErrCode = M_GetLastErrorCode();
       unsigned int    uiErrData = M_GetLastErrorData();
   	   LOGE("fcw_init : %d", uiErrCode);
       return ;
    }
	
	g_dDownscale = SCALE_RATIO;

    M_SetCamera_ID(0x30040505);//MCM-OV2710U ID : 0x30040505
    M_SetCamera_SensorUnitPixelSize( g_camera_sensor_width, g_camera_sensor_height );
    M_SetCamera_SensorDownscale( g_camera_sensor_scale );
    M_SetCamera_InputImageFormat( g_i_format );
	M_SetCamera_CameraImageSize( PREVIEW_WIDTH, PREVIEW_HEIGHT );
	M_SetCamera_CropImageSize( CROPPING_X, CROPPING_Y, CROPPING_WIDTH, CROPPING_HEIGHT );
	M_SetCamera_InputImageSize( ADAS_WIDTH, ADAS_HEIGHT );
	M_SetVehicle_CenterLine( adas_value[ADAS_VEHICLE_CENTER_LINE]);	
    M_SetCamera_InputFPS( (float)adas_value[ADAS_CAMERA_FRAME]/2 );
    M_SetCamera_LensEFL( g_lens_efl );
    M_SetCamera_TiltedAngle( camera_angle );
    M_SetCamera_HeightFromGround( adas_value[ADAS_CAMERA_HEIGHT]);
    M_SetCamera_CenterArrangement(adas_value[ADAS_CAMERA_CENTER_OFFSET]);
    M_SetVehicle_Width( adas_value[ADAS_CAR_WIDTH] );
    M_SetVehicle_Cam2WheelLength(adas_value[ADAS_CAMERA_2_WHEEL]);
    M_SetVehicle_Cam2BumperLength(adas_value[ADAS_CAM_2_BUMPER] );
    
    hood_line = (adas_value[ADAS_HOOD_LINE] - CROPPING_Y)/SCALE_RATIO;
    if ( hood_line < 0 ) hood_line = 0;
    else if ( hood_line >= ADAS_HEIGHT ) hood_line = ADAS_HEIGHT - 1;
    	
    M_SetVehicle_HoodLine(hood_line);

    M_SetLDW_ActiveVelocity(adas_value[ADAS_LDW_VELOCITY]); //minimum value 40 km/h.
    M_SetLDW_SensitivityL(adas_value[ADAS_LDW_SENSITY_LEFT]);
    M_SetLDW_SensitivityR(adas_value[ADAS_LDW_SENSITY_RIGHT]);

    M_SetFCW_ActiveVelocity( adas_value[ADAS_FCW_ACTIVITY_VELOCITY]); //above 5km/h
    M_SetFCW_TTC1( ((float)adas_value[ADAS_TTC])/10 ); //caution time to collision
    M_SetFCW_TTC2( 0.4f ); //alert time to collision
    M_SetFCW_NearWarningDistance(adas_value[ADAS_DISTANCE]); //300 cm
	
	M_SetLDW_EnableCalibration( 1, 1 );

    if( !M_Create() )
    {
       return;
    }
	CROPPING_X = M_GetCamera_CropImageLeft();
	version = M_GetVersion();
	if ( version ) next_tok = strtok(version,".");
	LOGD("Version 0 : %s\n",next_tok);
	next_tok = strtok(NULL,".");
	LOGD("Version 1 : %s\n",next_tok);
	if ( next_tok ) mdas_major = atoi(next_tok);
	next_tok = strtok(NULL,".");
	LOGD("Version 2 : %s\n",next_tok);
	if ( next_tok ) mdas_minor = atoi(next_tok);
	next_tok = strtok(NULL,".");
	LOGD("Version 3 : %s\n",next_tok);
	if ( next_tok ) mdas_build = atoi(next_tok);
	
}

void fcw_init (void)
{
	float camera_angle;
	if ( g_adas_init == 1 ) {
		g_adas_init = 0;
        camera_angle = fcw_get_camera_angle (adas_value[ADAS_VANISH_LINE] );
		LOGE("camera_angle %f",camera_angle);
		if ( camera_angle < -3.0 ) camera_angle = -3.0;
        fcw_close();
        fcw_init_algo(camera_angle);
	}
	else {
		fcw_init_algo(0.05f);
		camera_angle = fcw_get_camera_angle(adas_value[ADAS_VANISH_LINE]);
		if ( camera_angle < -3.0 ) camera_angle = -3.0;
		fcw_close();
		fcw_init_algo(camera_angle);
	}
	g_adas_init = 1;
}

void fcw_execute ( unsigned char *buffer) 
{
	int i,j;

	//LOGE("fcw_execute : after scaler");
	
	if ( (g_adas_init == 0) || (memcmp(&adas_value,&adas_value_cur,sizeof(adas_value_cur)) != 0) ){
		fcw_init();
		memcpy(&adas_value_cur,&adas_value,sizeof(adas_value));		
	}
	//ALOGE("valish_line=%d",adas_value[ADAS_VANISH_LINE]);

	if ( M_SetInputImage(buffer) ) {
		M_SetVehicle_CANData(g_can_data);
		M_ProcessCommon();
		M_ProcessScene();
		M_ProcessLDW();	
		M_ProcessFCW();
	}

#if 0
	{
		time_t current_time;
		struct tm *struct_time;
		char filename[1024];
		FILE *yuv_data_fp = NULL;

		if ( (frame_count%50)==0 ) {
			time(&current_time);
			struct_time = localtime(&current_time);
			sprintf(filename,"/data/yuv_data_%04d%02d%02d%02d%02d%02d.yuv",struct_time->tm_year+1900,struct_time->tm_mon+1,struct_time->tm_mday,
							     struct_time->tm_hour,struct_time->tm_min,struct_time->tm_sec);
			yuv_data_fp = fopen(filename,"w");
			if ( yuv_data_fp ) {
				fwrite(buffer,ADAS_WIDTH*ADAS_HEIGHT*3/2,1,yuv_data_fp);
				fflush(yuv_data_fp);
				fclose(yuv_data_fp);
			}
			else {
				LOGE("YUV file open failed");
			}
		}
	}
#endif

	if ( M_GetLDWResult() ) {
		int i,j;
		memcpy(&g_ldw_result,M_GetLDWResult(),sizeof(sResultLDW));
		g_ldw_result.autoCalVanishLine = g_ldw_result.autoCalVanishLine*SCALE_RATIO + CROPPING_Y;
		for ( j=0; j < 2; j++ ) {
			for ( i=0; i < g_ldw_result.lanes[j].count1; i++) {
				g_ldw_result.lanes[j].sx1[i] = g_ldw_result.lanes[j].sx1[i]*SCALE_RATIO + CROPPING_X;
				g_ldw_result.lanes[j].sy1[i] = g_ldw_result.lanes[j].sy1[i]*SCALE_RATIO + CROPPING_Y;
				g_ldw_result.lanes[j].ex1[i] = g_ldw_result.lanes[j].ex1[i]*SCALE_RATIO + CROPPING_X;
				g_ldw_result.lanes[j].ey1[i] = g_ldw_result.lanes[j].ey1[i]*SCALE_RATIO + CROPPING_Y;
			}
			for ( i=0; i < g_ldw_result.lanes[j].count2; i++) {
				g_ldw_result.lanes[j].sx2[i] = g_ldw_result.lanes[j].sx2[i]*SCALE_RATIO + CROPPING_X;
				g_ldw_result.lanes[j].sy2[i] = g_ldw_result.lanes[j].sy2[i]*SCALE_RATIO + CROPPING_Y;
				g_ldw_result.lanes[j].ex2[i] = g_ldw_result.lanes[j].ex2[i]*SCALE_RATIO + CROPPING_X;
				g_ldw_result.lanes[j].ey2[i] = g_ldw_result.lanes[j].ey2[i]*SCALE_RATIO + CROPPING_Y;
			}
		}
	}
	if ( M_GetFCWResult() ) {
		int i;
		memcpy(&g_fcw_result,M_GetFCWResult(),sizeof(sResultFCW));
		for ( i=0 ; i < g_fcw_result.count; i++ ) {
			g_fcw_result.x[i] = g_fcw_result.x[i]*SCALE_RATIO + CROPPING_X;
			g_fcw_result.y[i] = g_fcw_result.y[i]*SCALE_RATIO + CROPPING_Y;
			g_fcw_result.w[i] = g_fcw_result.w[i]*SCALE_RATIO;
			g_fcw_result.h[i] = g_fcw_result.h[i]*SCALE_RATIO;
		}
	}
}

//fengyun add for adas end

