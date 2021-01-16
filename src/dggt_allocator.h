#ifndef _DGGT_ALLOCATOR_H_
#define _DGGT_ALLOCATOR_H

#include <stddef.h>
#include "dggt_mem_utils.h"

namespace dggt::mem
{
	enum alloc_t
	{
		LINEAR_ALLOC,
		STACK_ALLOC,
		POOL_ALLOC,
		FREE_BLOCK_ALLOC
	};

	struct allocator
	{
		alloc_t type;
		void* memAddr;
		size_t memSize;
		size_t used;

		allocator(alloc_t allocType,void* mem,size_t size)
			: type(allocType),memAddr(mem),memSize(size),used(0) { }

		void* alloc_mem(size_t size);
		void free_mem(void* ptr,size_t size);
		void clear();
		size_t get_size();
		size_t get_used();
		size_t get_available();
		alloc_t get_type();

		template <typename T>
		T* alloc(size_t count)
		{
			return (T*)alloc_mem(sizeof(T)*count);
		}

		template <typename T>
		void free(void* ptr,size_t count)
		{
			return free_mem(ptr,sizeof(T)*count);
		}
	};
}

#endif
