#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h> 
#include <sdklog.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include"ion.h"
#include "DmaIon.h"

int dma_ion_open(int *ion_fd)
{
	int fd = open("/dev/ion", O_RDWR, 0);
    if(fd < 0){
        ALOGE("open /dev/ion err.\n");
        return -1;
    }
    *ion_fd = fd;
    return 0;
}

int dma_ion_sync(int ion_fd,int dmafd)
{
    struct ion_fd_data data2 ;
	data2.fd=dmafd;
    ioctl(ion_fd, ION_IOC_SYNC, &data2);
	return 0;
}
// return virtual address: 0 failed
ion_buffer_node dma_ion_alloc(int ion_fd,int size)
{
	int rest_size = 0;
	int addr_phy = 0;
	int addr_vir = 0;
	
	ion_buffer_node ion_buf;
	
	ion_buffer_node * alloc_buffer = &ion_buf;


	ion_allocation_data_t alloc_data;
	ion_handle_data_t handle_data;
	ion_custom_data_t custom_data;
	ion_fd_data_t fd_data;
	sunxi_phys_data phys_data;
	int fd, ret = 0;
	
	if(size <= 0)
	{
		ALOGE("can not alloc size 0");
		goto ALLOC_OUT;
	}
	fd = ion_fd;

	/*alloc buffer*/
	alloc_data.len = size;
	alloc_data.align = 4096 ;
	alloc_data.heap_id_mask = ION_HEAP_CARVEOUT_MASK;
	alloc_data.flags = ION_FLAG_CACHED | ION_FLAG_CACHED_NEEDS_SYNC;
	ret = ioctl(fd, ION_IOC_ALLOC, &alloc_data);
	if (ret)
	{
		ALOGE("ION_IOC_ALLOC error");
		goto ALLOC_OUT;
	}

	/*get physical address*/
	phys_data.handle = (void *)alloc_data.handle;
	custom_data.cmd = ION_IOC_SUNXI_PHYS_ADDR;
	custom_data.arg = (unsigned long)&phys_data;
	ret = ioctl(fd, ION_IOC_CUSTOM, &custom_data);
	if (ret)
	{
		ALOGE("ION_IOC_CUSTOM failed");
		goto out1;
	}
	addr_phy = phys_data.phys_addr;

	/*get dma buffer fd*/
	fd_data.handle = alloc_data.handle;
	ret = ioctl(fd, ION_IOC_MAP, &fd_data);
	if (ret)
	{
		ALOGE("ION_IOC_MAP failed");
		goto out1;
	}

	/*mmap to user space*/
	addr_vir = (int)mmap(NULL, alloc_data.len, PROT_READ | PROT_WRITE, MAP_SHARED, 
					fd_data.fd, 0);
	if ((int)MAP_FAILED == addr_vir)
	{
		ALOGE("mmap fialed");
		goto out2;
	}

	alloc_buffer->size	    = size;
	alloc_buffer->phy 	    = addr_phy;
	alloc_buffer->vir 	    = addr_vir;
	alloc_buffer->handle     = (void *)alloc_data.handle;
	alloc_buffer->dmabuf_fd = fd_data.fd;
	
	printf("alloc succeed, addr_phy: 0x%08x, addr_vir: 0x%08x, size: %d", addr_phy, addr_vir, size);

	//list_add_tail(&alloc_buffer->i_list, &g_alloc_context->list);

	goto ALLOC_OUT;
out3:
	/* unmmap */
	ret = munmap((void*)addr_vir, alloc_data.len);
	if(ret) printf("munmap err, ret %d\n", ret);
	printf("munmap succes\n");
	
out2:
	/* close dmabuf fd */
	::close(fd_data.fd);
	printf("close dmabuf fd succes\n");

out1:
	/* free buffer */
	handle_data.handle = alloc_data.handle;
	ret = ioctl(fd, ION_IOC_FREE, &handle_data);
	if(ret)
		printf("ION_IOC_FREE err, ret %d\n", ret);
	printf("ION_IOC_FREE succes\n");

ALLOC_OUT:
	
	//pthread_mutex_unlock(&g_mutex_alloc);
	//::close(fd_data.fd);
	//::close(fd);

	return ion_buf;
	
}
