// TODO: POOL_ALLOC init function
#ifndef _DGGT_ALLOCATOR_H_
#define _DGGT_ALLOCATOR_H_

#include "dggt_defs.h" 
#include "dggt_mem_utils.h"
#include "dggt_math.h"

namespace dggt::mem
{
	struct allocator;

	enum alloc_t
	{
		LINEAR_ALLOC,
		STACK_ALLOC,
		POOL_ALLOC,
		FREE_BLOCK_ALLOC,
		AUTOSTACK_ALLOC
	};

	typedef size_t stack_state;

	typedef allocator autostack_alloc;
	typedef allocator stack_alloc;

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
			struct
			{
				stack_alloc* stackAlloc;
			};
		};


		allocator(alloc_t allocType,void* mem,size_t size);
		allocator(void* mem,size_t size,size_t poolSize);
		allocator(stack_alloc* stackAlloc);

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
		void clear_buff();

		template <typename T>
		T* alloc(size_t count=0)
		{
			return (T*)alloc_mem(sizeof(T)*count);
		}

		template <typename T>
		void free(T* ptr,size_t count=0)
		{
			return free_mem(ptr,sizeof(T)*count);
		}

		~allocator();
	};

}

#endif
