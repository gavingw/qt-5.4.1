//fengyun add for adas start
#ifndef __ADAS_PROCESS_H__
#define __ADAS_PROCESS_H__
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "MDASLib.h"
#include "MDASLibTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
	ADAS_CAN_DATA_SPEED = 0,//CAN data and speed
	ADAS_FRAME_WIDTH, //
	ADAS_FRAME_HEIGHT,
	ADAS_APP_DISP_WIDTH,
	ADAS_APP_DISP_HEIGHT,
	ADAS_CAMERA_FRAME,
	ADAS_CAMERA_HEIGHT,
	ADAS_TTC,
	ADAS_VANISH_LINE,
	ADAS_CAMERA_CENTER_OFFSET,
	ADAS_CAR_WIDTH,
	ADAS_CAMERA_2_WHEEL,
	ADAS_CAM_2_BUMPER,
	ADAS_LDW_SENSITY_LEFT,
	ADAS_LDW_SENSITY_RIGHT,
	ADAS_HOOD_LINE,
	ADAS_DISTANCE,
	ADAS_LDW_VELOCITY,//g_ldw_velocity
	ADAS_FCW_ACTIVITY_VELOCITY,//g_fcw_activity_velocity
	ADAS_VEHICLE_CENTER_LINE,
	ADAS_TOTAL_NUM, //add item before ADAS_TOTOAL_NUM
};

int fcw_setAdasParameters(int key,int value);
void fcw_close(void);
void fcw_execute(unsigned char *buf);
void fcw_init (void);
extern sResultLDW g_ldw_result;
extern sResultFCW g_fcw_result;
extern int mdas_major;
extern int mdas_minor;
extern int mdas_build;


extern int PREVIEW_WIDTH;
extern int PREVIEW_HEIGHT;
extern int CROPPING_X;
extern int CROPPING_Y;
extern int CROPPING_WIDTH;
extern int CROPPING_HEIGHT;
extern int ADAS_WIDTH;
extern int ADAS_HEIGHT;
extern float SCALE_RATIO;

#ifdef __cplusplus
}
#endif

#endif  /* __STREAM_DECORDER_H__ */

//fengyun add for adas end

