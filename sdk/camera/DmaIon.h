#ifndef _DMAION_H_
#define _DMAION_H_

typedef struct ION_BUFFER_NODE
{
	//struct list_head i_list; 
	int phy;		//phisical address
	int vir;		//virtual address
	int size;		//buffer size
	int dmabuf_fd;	//dma_buffer fd
	void* handle;		//alloc data handle
}ion_buffer_node;

int dma_ion_open(int *ion_fd);
int dma_ion_sync(int ,int );
ion_buffer_node dma_ion_alloc(int ion_fd, int size);

#endif