#include "dggt_mem_utils.h"

namespace dggt::mem
{
	void* alloc_mem(size_t size)
	{
		return mmap(0,size,PROT_READ|PROT_WRITE,
				MAP_ANONYMOUS|MAP_SHARED,-1,0);
	}

	void free_mem(void* ptr,size_t size)
	{
		munmap(ptr,size);
	}
}
