#ifndef _DGGT_MEM_UTILS_H_
#define _DGGT_MEM_UTILS_H_

#include <stddef.h>
#include <sys/mman.h>

namespace dggt::mem
{
	inline void* ptr_add(void* ptr,size_t offset)
	{
		return (void*)((unsigned char*)ptr+offset);
	}

	inline void* ptr_sub(void* ptr,size_t offset)
	{
		return (void*)((unsigned char*)ptr-offset);
	}

	void* alloc_mem(size_t size);
	void free_mem(void* ptr,size_t size);

	template <typename T>
	T* alloc(size_t count)
	{
		return (T*)alloc_mem(sizeof(T)*count);
	}

	template <typename T>
	void free(T* ptr,size_t count)
	{
		free_mem((void*)ptr,sizeof(T)*count);
	}
}

#endif
