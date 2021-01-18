// TODO: POOL_ALLOC init function
#ifndef _DGGT_ALLOCATOR_H_
#define _DGGT_ALLOCATOR_H

#include "dggt_defs.h" 
#include "dggt_mem_utils.h"
#include "dggt_math.h"

namespace dggt::mem
{
	enum alloc_t
	{
		LINEAR_ALLOC,
		STACK_ALLOC,
		POOL_ALLOC,
		FREE_BLOCK_ALLOC
	};

	typedef size_t stack_state;

	struct pool_block
	{
		pool_block* next;
	};

	struct free_block
	{
		size_t size;
		free_block* next;
	};

	struct allocator
	{
		alloc_t type;
		void* memAddr;
		size_t memSize;
		size_t used;
		union
		{
			stack_state state;
			struct
			{
				size_t pSize;
				pool_block* poolHead;
			};
			struct
			{
				free_block* freeHead;
			};
		};

		allocator(alloc_t allocType,void* mem,size_t size)
			: type(allocType),memAddr(mem),
			memSize(size),used(0),state(0){ }

		allocator(alloc_t allocType,void* mem,size_t size,
				size_t poolSize)
			: allocator(allocType,mem,size)
		{
			pSize=poolSize;
			if (pSize>0)
			{
				size_t blockCount=size/pSize;
				for (int i=0;i<blockCount;++i)
				{
					pool_block* newBlock=
						(pool_block*)ptr_add(
								mem,i*pSize);
					newBlock->next=poolHead;
					poolHead=newBlock;
				}
			}
		}

		void* alloc_mem(size_t size=0);
		void free_mem(void* ptr,size_t size=0);
		void clear();
		size_t get_size();
		size_t get_used();
		size_t get_available();
		alloc_t get_type();
		bool32 owns(void* ptr,size_t size);
		stack_state save_state();
		void restore_state(stack_state state);

		template <typename T>
		T* alloc(size_t count=0)
		{
			return (T*)alloc_mem(sizeof(T)*count);
		}

		template <typename T>
		void free(void* ptr,size_t count=0)
		{
			return free_mem(ptr,sizeof(T)*count);
		}
	};
}

#endif
