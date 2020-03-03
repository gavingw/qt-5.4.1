#ifdef FB_INIT
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/videodev2.h>

int main()
{
    int ret = 0;
    int fd;
    struct fb_var_screeninfo var;
	struct fb_fix_screeninfo finfo;  
    if((fd = open("/dev/fb0",O_RDWR)) == -1)
    {
        printf("open file /dev/fb0 fail. \n");
        return 0;
    }
    ioctl(fd,FBIOGET_FSCREENINFO,&finfo);
	int screensize = finfo.smem_len;  
   
   /* Map the device to memory */  
     char *  frameBuffer =  
           (char *) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,  
                           fd, 0);  
						   
					   
	memset(frameBuffer,0,screensize);	
	munmap (frameBuffer, screensize);   //解除内存映射，与mmap对应  
    if (ioctl(fd,FBIOGET_VSCREENINFO,&var))
    {
        printf("Error reading variable information/n");
    }

    if(ioctl(fd,FBIOGET_VSCREENINFO,&var)==-1)
    {
        printf("get screen information failure\n");
        return -2;
    }
    {
        var.bits_per_pixel = 32;
        var.transp.length = 8;
        var.red.length = 8;
        var.green.length = 8;
        var.blue.length = 8;
        var.blue.offset = 0;
        var.green.offset = var.blue.offset + var.blue.length;
        var.red.offset = var.green.offset + var.green.length;
        var.transp.offset = var.red.offset + var.red.length;

    }
    if(ioctl(fd,FBIOPUT_VSCREENINFO,&var)==-1)
    {
        printf("put screen information failure\n");
        return -2;
    }

    close(fd);
}
#endif