/// @file dggt_mem_utils.h
/// @brief Contains memory utility functions.
#ifndef _DGGT_MEM_UTILS_H_
#define _DGGT_MEM_UTILS_H_

#include <stddef.h>
#include <sys/mman.h>

namespace dggt::mem
{
	/**
		Adds an offset to a memory address.
		@param ptr Pointer to memory.
		@param offset Offset in bytes to add to address.
		@returns The address offset bytes away from ptr.
	  */
	inline void* ptr_add(void* ptr,size_t offset)
	{
		return (void*)((unsigned char*)ptr+offset);
	}

	inline void* ptr_sub(void* ptr,size_t offset)
	{
		return (void*)((unsigned char*)ptr-offset);
	}

	inline size_t addr_diff(void* lhs,void* rhs)
	{
		if (rhs<lhs)
		{
			void* temp=rhs;
			rhs=lhs;
			lhs=temp;
		}
		return (size_t)rhs-(size_t)lhs;
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
