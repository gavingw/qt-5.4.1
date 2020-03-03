#ifndef _HWDISPLAY2_H
#define _HWDISPLAY2_H
#include <semaphore.h>
#include "Mutex.h"
#define SCREEN_0 0
#define SCREEN_1 1
#define SCREEN_2 2
#define SCREEN_NUM SCREEN_2
#define LAYER_NUM 4
#define CHANNEL_NUM 4

//for video
#define S_LAYER_ID 0
#define S_CHANNEL_ID 0

//for screen 1
#define FB1_LAYER_ID 1
#define FB1_CHANNEL_ID 1

//for screen 0
//fb0
#define FB_CHANNEL_ID 1
#define FB_LAYER_ID 0

#define DISP_DEV "/dev/disp"
#define RET_OK 0
#define RET_FAIL -1
#define COMP_LAYER_HDL(s,c,layer) (((s&0xf)<<16)|((c&0xf)<<8)|(layer&0xf))
#define HDL_2_SCREENID(hdl) ((hdl>>16)&0xf)
#define HDL_2_CHANNELID(hdl) ((hdl>>8)&0xf)
#define HDL_2_LAYERID(hdl) ((hdl)&0xf)


#include "sunxi_display2.h"
struct view_info
{
    int x;
    int y;
    unsigned int w;
    unsigned int h;
};

struct src_info
{
    unsigned int w;
    unsigned int h;
    unsigned int format;
};

namespace android
{

class HwDisplay
{
public:
    static HwDisplay* getInstance();
	HwDisplay& operator=(const HwDisplay&);
    HwDisplay(const HwDisplay&);
    HwDisplay();
    ~HwDisplay();
	
    //int tvout(int ch,int scn_w,int scn_h);
    int lcdxres;
    int lcdyres;
	
	int hwd_init(void);

    int hwd_exit(void);
    int layer_init();
    int layer_init(int screen_id,unsigned int channel,unsigned int layer_id);

    int hwd_video_start(unsigned int hlay);
    int hwd_video_start(int screen_id,unsigned int channel,unsigned int layer_id);
    int hwd_screen1_init();
    int hwd_screen1_deinit();
    int hwd_screen1_mode(int mode);
    int mScreen1Mode;
    int mScreen0Mode;
    int hwd_video_set_addr(unsigned int hlay,int csi_format,int width, int height, void *addr0,void *addr1);

    int hwd_video_set_addr(int screen_id,unsigned int channel,unsigned int layer_id,int csi_format,int width, int height,void*addr0,void *addr1);
    int hwd_video_stop(unsigned int hlay);
    int hwd_video_stop(int screen_id,unsigned int channel,unsigned int layer_id);
    int hwd_layer_set_src(unsigned int hlay, struct src_info *src);

    int hwd_layer_set_rect(unsigned int hlay, struct view_info *view);

    int hwd_layer_set_rect(int screen_id,unsigned int channel,unsigned int layer_id, struct view_info *view);

    int hwd_tvout(void);
    int hwd_hdmiout(void);
    int hwd_other_screen(int screen, int type, int mode);
	int hwd_video_set_crop(unsigned int hlay,struct view_info crop);//temp use above function

//disp func call seq:
//request ->setsrc->setrect(maybe)->open->render(maybe if disp addr every change)->close->releases
    int aut_hwd_layer_request(struct view_info* surface,int screen_id,unsigned int channel,unsigned int layer_id);
    int aut_hwd_layer_set_src(unsigned int hlay, struct src_info *src,int addr0);
    int aut_hwd_layer_set_rect(unsigned int hlay, struct view_info *src_rect);// impl todo
    int aut_hwd_layer_set_zorder(unsigned int hlay,int zorder);
	int aut_hwd_layer_open(unsigned int hlay);
	
	int aut_hwd_layer_fb_mode(int screen_id,unsigned int type  ,unsigned int mode);
	int aut_hwd_layer_sufaceview(unsigned int hlay,struct view_info* surface);
	int aut_hwd_layer_set_alpha(unsigned int hlay,int alpha_mode,int alpha_value);

    int aut_hwd_layer_render(unsigned int hlay, void*addr0,void *addr1);

    int aut_hwd_layer_close(unsigned int hlay);
	int aut_hwd_layer_release(unsigned int hlay);

private:

    int	mDisp_fd;
    int mLayer;
	int mScreen;
    struct view_info mView;
    struct view_info mView1;

    static bool		mInitialized;
    static HwDisplay *sHwDisplay;
    static Mutex sLock;
    disp_layer_config config;//old use before v1.0.1
    disp_layer_config layer_cfg[SCREEN_NUM][CHANNEL_NUM][LAYER_NUM];//new 1.1.0
    int layer_stat[SCREEN_NUM][CHANNEL_NUM][LAYER_NUM];//new 1.1.0
	int hwd_setfbcfg(int type,int mode);

};
}
#endif


