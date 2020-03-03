/*
 * Copyright (c) 2011-2013 Luc Verhaegen <libv@skynet.be>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * Hello triangle, adapted for native display on libMali.so.
 * only for test dmabuf ,so change you own code
 */
#ifdef DMAFD_TEST
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/types.h>
#include <errno.h>


#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <sys/time.h>
#include <time.h>
#include "ion.h"
#include "drm_fourcc.h"
#include <pthread.h>
#include <semaphore.h>


#define EGL_EGLEXT_PROTOTYPES
#include <EGL/eglext.h>

#define SC_WIDTH 1024
#define SC_HEIGHT 600

static GLuint yuvTex;
static GLint gYuvTexSamplerHandle;
#if 0
struct fbdev_window
{
	unsigned short width;
	unsigned short height;
};
#endif
struct fbdev_window native_window;


GLuint gProgram;

static const char *vertex_shader_source =
	"attribute vec4 aPosition;    \n"
	"attribute vec2 TexCoords;    \n"
	"varying vec2 yuvTexCoords;\n"
	"void main()                  \n"
	"{                            \n"
	"    yuvTexCoords = TexCoords ;\n"
	"    gl_Position = aPosition; \n"
	"}                            \n";

static const char *fragment_shader_source = 
    "#extension GL_OES_EGL_image_external : require\n"
    "precision mediump float;\n"
    "uniform samplerExternalOES yuvTexSampler;\n"
    "varying vec2 yuvTexCoords;\n"
    "void main() {\n"
    "  gl_FragColor = texture2D(yuvTexSampler, yuvTexCoords);\n"
    "}\n";
#if 0
//fk
static GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f,
			       -0.5f, -0.5f, 0.0f,
				0.5f, -0.5f, 0.0f };
				#else
static GLfloat vVertices[] = {  
			       -1.0f, -1.0f, 0.0f,
				1.0f, -1.0f, 0.0f,
-1.0f,  1.0f, 0.0f,
1.0f,  1.0f, 0.0f,
};
static GLfloat vTextcoords[] = {	
				0.0f, 1.0f,
				1.0f, 1.0f, 
				0.0f, 0.0f,
				1.0f,0.0f,
};

#endif
static EGLint const config_attribute_list[] = {
	EGL_RED_SIZE, 8,
	EGL_GREEN_SIZE, 8,
	EGL_BLUE_SIZE, 8,
	EGL_ALPHA_SIZE, 8,
	EGL_BUFFER_SIZE, 32,

	EGL_STENCIL_SIZE, 0,
	EGL_DEPTH_SIZE, 0,
	EGL_SAMPLE_BUFFERS, 1 ,
	EGL_SAMPLES, 4,

	EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,

	EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_PIXMAP_BIT,


	EGL_NONE
};

static EGLint window_attribute_list[] = {
	EGL_NONE
};

static const EGLint context_attribute_list[] = {
	EGL_CONTEXT_CLIENT_VERSION, 2,
	EGL_NONE
};

EGLDisplay egl_display;
EGLSurface egl_surface;
long long pts_last=0;

#if 0
static void Redraw(int width, int height)
{
	struct timeval tv;
    EGLSyncKHR sync;
    long time;
    static int whiel = 0;
    
	glViewport(0, 0, width, height);
	#if 0
    if((whiel++%20)>10) {
        glClearColor(0.5, 0.5, 0.5, 1.0);
        eglSwapBuffers(egl_display, egl_surface);
	    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        printf("time: %ld ms. 3333\n", time);
        return;
    }
	#endif
	gettimeofday( &tv, NULL);
    time = 1000 * (long)tv.tv_sec + (long)tv.tv_usec/1000;
   // printf("time_s: %ld ms. 1111\n", time);


    sync = eglCreateSyncKHR(egl_display, EGL_SYNC_FENCE_KHR, NULL);
    eglClientWaitSyncKHR(egl_display, sync, EGL_SYNC_FLUSH_COMMANDS_BIT_KHR, 3000000000);
    eglDestroySyncKHR(egl_display, sync);
    if(glGetError())
        printf("Error:44 failed: 0x%08X: \"%s\"\n",glGetError()); 
    gettimeofday( &tv, NULL);
    time = 1000 * (long)tv.tv_sec + (long)tv.tv_usec/1000;
    //printf("time_s: %ld ms. 222\n", time);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    if(glGetError())
        printf("Error:44 failed: 0x%08X: \"%s\"\n",glGetError()); 

	eglSwapBuffers(egl_display, egl_surface);

}
#endif

#define WITH 1440
#define HEIGHT 960
int ion_fd = -1;
#if 0
int init_dma_buf(char *path, int size)
{
    int file_fd;
    int ret;
    void* addr_file,*addr_dma;

printf("deal######## %s ####size=%d #####.\n",path,size);
    if(ion_fd == -1)
        ion_fd = open("/dev/ion", O_RDWR);
    if(ion_fd < 0)
    {
        printf("open /dev/ion err.\n");
        return -1;
    }
    //alloc
    struct ion_allocation_data sAllocInfo =
	{
		.len		= size,
		.align		= 4096,
		.heap_id_mask	= ION_HEAP_CARVEOUT_MASK,//ION_HEAP_TYPE_DMA_MASK,
		.flags		= 0,
		.handle     = 0,
	};
	ret = ioctl(ion_fd, ION_IOC_ALLOC, &sAllocInfo);
	if(ret < 0)
	{
	    printf("alloc ion err.\n");
		close(ion_fd);
	    return  -1;
	}
    //share
    struct ion_fd_data data = {
        .handle = sAllocInfo.handle,
    };

    //ret = ioctl(ion_fd, ION_IOC_SHARE, &data);//ION_IOC_MAP
	ret = ioctl(ion_fd, ION_IOC_MAP, &data);
    if (ret < 0)
        return ret;
    if (data.fd < 0) {
        printf("share ioctl returned negative fd\n");
        return -1;
    }
    addr_dma = (void *)mmap((void *)0, size, PROT_WRITE, MAP_SHARED, data.fd, 0);
    //load textue
    file_fd = open(path,O_RDWR);
    if(file_fd < 0){
        printf("open %s err.\n",path);
        return -1;
    }
    addr_file = (void *)mmap((void *)0, size, PROT_WRITE, MAP_SHARED, file_fd, 0);
    memcpy(addr_dma, addr_file , size);
    //sync for dma gpu
    struct ion_fd_data data2 = {
        .fd = data.fd,
    };
    
    ioctl(ion_fd, ION_IOC_SYNC, &data2);
    munmap(addr_file ,size);
    munmap(addr_dma ,size);
    printf("end######## %s #########.\n",path);


    return data.fd;
}
#endif
static int camera_dmafd=0;
static int camera_texure_init=0;
int setupTexture(EGLDisplay dpy, EGLContext context) {
    int dma_fd;
	int atti = 0;
    EGLint attribs[30];


    dma_fd = camera_dmafd;//init_dma_buf("/tmp/yuv420_y.bin",HEIGHT*WITH);
    if(dma_fd < 0)
    {
        printf("init yuv420_y err.\n");
	    return -1;
    }

    attribs[atti++] = EGL_WIDTH;
	attribs[atti++] = WITH;
	attribs[atti++] = EGL_HEIGHT;
	attribs[atti++] = HEIGHT;
	attribs[atti++] = EGL_LINUX_DRM_FOURCC_EXT;
	attribs[atti++] = DRM_FORMAT_NV21;
	attribs[atti++] = EGL_DMA_BUF_PLANE0_FD_EXT;
	attribs[atti++] = dma_fd;
	attribs[atti++] = EGL_DMA_BUF_PLANE0_OFFSET_EXT;
	attribs[atti++] = 0;
	attribs[atti++] = EGL_DMA_BUF_PLANE0_PITCH_EXT;
	attribs[atti++] = WITH;

	attribs[atti++] = EGL_DMA_BUF_PLANE1_FD_EXT;
	attribs[atti++] = dma_fd;
	attribs[atti++] = EGL_DMA_BUF_PLANE1_OFFSET_EXT;
	attribs[atti++] = WITH*HEIGHT;
	attribs[atti++] = EGL_DMA_BUF_PLANE1_PITCH_EXT;
	attribs[atti++] = WITH;

    attribs[atti++] = EGL_YUV_COLOR_SPACE_HINT_EXT;
    attribs[atti++] = EGL_ITU_REC709_EXT;
    attribs[atti++] = EGL_SAMPLE_RANGE_HINT_EXT;
    attribs[atti++] = EGL_YUV_FULL_RANGE_EXT;

    attribs[atti++] = EGL_NONE;
    EGLImageKHR img = eglCreateImageKHR(dpy, EGL_NO_CONTEXT, EGL_LINUX_DMA_BUF_EXT, 0, attribs); 
    if (img == EGL_NO_IMAGE_KHR) {
       printf("Error:0000 failed: 0x%08X\n",glGetError()); 

        return -1;
    }
    if(glGetError())
        printf("Error:33 failed: 0x%08X\n",glGetError()); 
    glGenTextures(1, &yuvTex);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, yuvTex);
    glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, (GLeglImageOES)img);
    if(glGetError())
        printf("Error:44 failed: 0x%08X\n",glGetError()); 
    return 0;
}

sem_t sem_frame;
int sem_frame_init=0;
int info_aframe(int *dmafd)
{
	if(sem_frame_init==1){
		camera_dmafd =*dmafd;
		sem_post(&sem_frame);
	//	printf("post sem \r\n");
		
	}
}

#define DMAFD_W 1440
#define DMAFD_H 960 

typedef struct eglimg_dmafd_
{
	EGLImageKHR eglimg;
	int dmafd;
}eglimg_dmafd_t;

#define MAX_DMAFD_NUM 30

static eglimg_dmafd_t g_eglimg_dmafd_array[MAX_DMAFD_NUM];
static GLuint s_yuvTex;

static int search_dmafd(int dmafd, EGLImageKHR *p_img, int *p_index)
{
	int ret=-1;
	int i;

	for(i=0; i<MAX_DMAFD_NUM; i++)
	{
		if(g_eglimg_dmafd_array[i].dmafd == -1)
		{
			*p_index = i;
			return -1;
		}
		else if(g_eglimg_dmafd_array[i].dmafd == dmafd)
		{
			*p_img = g_eglimg_dmafd_array[i].eglimg;
			*p_index = i;
			return 0;
		}
	}

	return ret;
}

static int SetupDmafd(EGLDisplay dpy, int dma_fd, EGLImageKHR *p_img ) 
{
	int atti = 0;
	EGLint attribs[30];
	int ret;

	if(dma_fd < 0)
	{
		printf("damfd yuv420_y err.\n");
		return -1;
	}

	attribs[atti++] = EGL_WIDTH;
	attribs[atti++] = DMAFD_W ;
	attribs[atti++] = EGL_HEIGHT;
	attribs[atti++] = DMAFD_H;
	attribs[atti++] = EGL_LINUX_DRM_FOURCC_EXT;
	attribs[atti++] = DRM_FORMAT_NV21;
	attribs[atti++] = EGL_DMA_BUF_PLANE0_FD_EXT;
	attribs[atti++] = dma_fd;
	attribs[atti++] = EGL_DMA_BUF_PLANE0_OFFSET_EXT;
	attribs[atti++] = 0;
	attribs[atti++] = EGL_DMA_BUF_PLANE0_PITCH_EXT;
	attribs[atti++] = DMAFD_W;

	attribs[atti++] = EGL_DMA_BUF_PLANE1_FD_EXT;
	attribs[atti++] = dma_fd;
	attribs[atti++] = EGL_DMA_BUF_PLANE1_OFFSET_EXT;
	attribs[atti++] = DMAFD_W * DMAFD_H ;
	attribs[atti++] = EGL_DMA_BUF_PLANE1_PITCH_EXT;
	attribs[atti++] = DMAFD_W;

	attribs[atti++] = EGL_YUV_COLOR_SPACE_HINT_EXT;
	attribs[atti++] = EGL_ITU_REC709_EXT;
	attribs[atti++] = EGL_SAMPLE_RANGE_HINT_EXT;
	attribs[atti++] = EGL_YUV_FULL_RANGE_EXT;

	attribs[atti++] = EGL_NONE;
	EGLImageKHR img = eglCreateImageKHR(dpy, EGL_NO_CONTEXT, EGL_LINUX_DMA_BUF_EXT, 0, attribs); 
	if (img == EGL_NO_IMAGE_KHR) 
	{
		printf("Error:0000 failed: 0x%08X\n",glGetError()); 
		return -1;
	}
	*p_img = img;
	printf("init glEGLImageTargetTexture2DOES yuvTex_dmafd:%p,  ok\n",img);
	return 0;
}

static int DmafdConnect(int dmafd)
{
	int ret;
	EGLImageKHR img;
	int index;
	ret = search_dmafd(dmafd, &img, &index);
	if(ret != 0)
	{
		ret = SetupDmafd(egl_display, dmafd, &img );
		if(ret != 0)
		{
			printf("SetupDmafd err\n");
		}
		else
		{
			g_eglimg_dmafd_array[index].dmafd = dmafd;
			g_eglimg_dmafd_array[index].eglimg = img;	
		}
	}

	glBindTexture(GL_TEXTURE_EXTERNAL_OES, s_yuvTex);
	glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, (GLeglImageOES)img);
	ret = glGetError();
	if(ret)
	{
		printf("Error:glEGLImageTargetTexture2DOES dmafd:%d, img:%p, texture:%x failed: 0x%08X\n",dmafd, img, s_yuvTex, ret); 
		return -1;
	}
	
	return 0;
}

int texture_main(void *param)
{
	EGLint egl_major, egl_minor;
	EGLConfig config;
	EGLint num_config;
	EGLContext context;
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program;
	GLint ret;
	GLint width, height;
    GLenum erro_status;
	egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (egl_display == EGL_NO_DISPLAY) {
		fprintf(stderr, "Error: No display found!\n");
		return -1;
	}

	if (!eglInitialize(egl_display, &egl_major, &egl_minor)) {
		fprintf(stderr, "Error: eglInitialise failed!\n");
		return -1;
	}

	printf("EGL Version: \"%s\"\n",
	       eglQueryString(egl_display, EGL_VERSION));
	printf("EGL Vendor: \"%s\"\n",
	       eglQueryString(egl_display, EGL_VENDOR));
	printf("EGL Extensions: \"%s\"\n",
	       eglQueryString(egl_display, EGL_EXTENSIONS));

	eglChooseConfig(egl_display, config_attribute_list, &config, 1,
			&num_config);

	context = eglCreateContext(egl_display, config, EGL_NO_CONTEXT,
				   context_attribute_list);
	if (context == EGL_NO_CONTEXT) {
		fprintf(stderr, "Error: eglCreateContext failed: 0x%08X\n",
			eglGetError());
		return -1;
	}
	native_window.width = SC_WIDTH;
	native_window.height = SC_HEIGHT;

	egl_surface = eglCreateWindowSurface(egl_display, config,
					     &native_window,
					     window_attribute_list);
	if (egl_surface == EGL_NO_SURFACE) {
		fprintf(stderr, "Error: eglCreateWindowSurface failed: "
			"0x%08X\n", eglGetError());
		return -1;
	}

	if (!eglQuerySurface(egl_display, egl_surface, EGL_WIDTH, &width) ||
	    !eglQuerySurface(egl_display, egl_surface, EGL_HEIGHT, &height)) {
		fprintf(stderr, "Error: eglQuerySurface failed: 0x%08X\n",
			eglGetError());
		return -1;
	}
	printf("Surface size: %dx%d\n", width, height);

	if (!eglMakeCurrent(egl_display, egl_surface, egl_surface, context)) {
		fprintf(stderr, "Error: eglMakeCurrent() failed: 0x%08X\n",
			eglGetError());
		return -1;
	}

	printf("GL Vendor: \"%s\"\n", glGetString(GL_VENDOR));
	printf("GL Renderer: \"%s\"\n", glGetString(GL_RENDERER));
	printf("GL Version: \"%s\"\n", glGetString(GL_VERSION));
	printf("GL Extensions: \"%s\"\n", glGetString(GL_EXTENSIONS));
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	if (!vertex_shader) {
		fprintf(stderr, "Error: glCreateShader(GL_VERTEX_SHADER) "
			"failed: 0x%08X\n", glGetError());
		return -1;
	}

	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &ret);
	if (!ret) {
		char *log;

		fprintf(stderr, "Error: vertex shader compilation failed!\n");
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &ret);

		if (ret > 1) {
			log = (char *)malloc(ret);
			glGetShaderInfoLog(vertex_shader, ret, NULL, log);
			fprintf(stderr, "%s", log);
			free ( log );  
		}
		glDeleteShader ( vertex_shader );
		return -1;
	}

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fragment_shader) {
		fprintf(stderr, "Error: glCreateShader(GL_FRAGMENT_SHADER) "
			"failed: 0x%08X\n", glGetError());
		return -1;
	}

	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &ret);
	if (!ret) {
		char *log;

		fprintf(stderr, "Error: fragment shader compilation failed!\n");
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &ret);

		if (ret > 1) {
			log = (char *)malloc(ret);
			glGetShaderInfoLog(fragment_shader, ret, NULL, log);
			fprintf(stderr, "%s", log);
			free ( log ); 
		}
		glDeleteShader ( fragment_shader );
		return -1;
	}

	program = glCreateProgram();
	if (!program) {
		fprintf(stderr, "Error: failed to create program!\n");
		return -1;
	}

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glBindAttribLocation(program, 0, "aPosition");
	glBindAttribLocation(program, 1, "TexCoords");

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &ret);
	if (!ret) {
		char *log;

		fprintf(stderr, "Error: program linking failed!\n");
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &ret);

		if (ret > 1) {
			log = (char *)malloc(ret);
			glGetProgramInfoLog(program, ret, NULL, log);
			fprintf(stderr, "%s", log);
			free ( log ); 
		}
		glDeleteShader ( program );
		return -1;
	}
	glUseProgram(program);
    
    if((erro_status = glGetError()))
       printf("Error: 6666 failed: 0x%08X\n",erro_status); 

    gYuvTexSamplerHandle = glGetUniformLocation(program, "yuvTexSampler");
    glUniform1i(gYuvTexSamplerHandle, 0);

    if((erro_status = glGetError()))
       printf("Error: 88 failed: 0x%08X\n",erro_status); 

    glClearColor(0.2, 0.2, 0.2, 1.0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, vTextcoords);
	glEnableVertexAttribArray(1);
    if((erro_status = glGetError()))
       printf("Error:77 failed: 0x%08X\n",erro_status); 
	
    setupTexture(egl_display,context);
	
	glGenTextures(1, &s_yuvTex);

	unsigned int i;
	for(i=0; i<MAX_DMAFD_NUM; i++)
	{
		g_eglimg_dmafd_array[i].dmafd = -1;
		g_eglimg_dmafd_array[i].eglimg = (void *)-1;
	}
	
    int res = sem_init(&sem_frame, 0, 0);
	 if (res != 0)
	 {
	  	perror("Semaphore initialization failed");
	 }
	 sem_frame_init=1;
	 EGLSyncKHR sync;
	while(1){
		sem_wait(&sem_frame); 
		#if 0
		//printf("wait sem \r\n");
		if((camera_texure_init==0)&&(camera_dmafd!=0)){
			if(setupTexture(egl_display,NULL) < 0)
        		printf("setupTexture err.\n");
			else
				camera_texure_init =1;
		}
		Redraw(width, height);
		#else
		DmafdConnect(camera_dmafd);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glFlush();
		
		eglSwapBuffers(egl_display, egl_surface);
		sync = eglCreateSyncKHR(egl_display, EGL_SYNC_FENCE_KHR, NULL);
		eglClientWaitSyncKHR(egl_display, sync, EGL_SYNC_FLUSH_COMMANDS_BIT_KHR, 3000000000);
		eglDestroySyncKHR(egl_display, sync);
		int retx;
		if(retx = glGetError())
			printf("Error:44 failed: 0x%08X: \"%s\"\n",retx); 
		#endif
	}
	return 0;
}
int create_textture()
{
    pthread_attr_t attr; 
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    errno = 0;
    pthread_t thread;
    int result = pthread_create(&thread, &attr,
                    (void* (*)(void*))texture_main, NULL);
    pthread_attr_destroy(&attr);


}
#endif
