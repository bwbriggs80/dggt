// TODO: templatize type enum??
#ifndef _DGGT_ALLOCATOR_H_
#define _DGGT_ALLOCATOR_H_

#include "dggt_defs.h" 
#include "dggt_mem_utils.h"
#include "dggt_math.h"

namespace dggt::mem
{
	template <msize Size=0>
	struct allocator_;

	enum alloc_t
	{
		LINEAR_ALLOC,
		STACK_ALLOC,
		POOL_ALLOC,
		FREE_BLOCK_ALLOC,
		AUTOSTACK_ALLOC
	};

	typedef msize stack_state;

	typedef allocator_<0> allocator;
	typedef allocator_<0> autostack_alloc;
	typedef allocator_<0> stack_alloc;

	struct pool_block
	{
		pool_block* next;
	};

	struct free_block
	{
		msize size;
		free_block* next;
	};

	template <>
	struct allocator_<0>
	{
		alloc_t type;
		void* memAddr;
		msize memSize;
		msize used;
		union
		{
			stack_state state;
			struct
			{
				msize pSize;
				pool_block* poolHead;
			};
			struct
			{
				free_block* freeHead;
			};
			struct
			{
				stack_alloc* stkAlloc;
			};
		};


		allocator_(alloc_t allocType,void* mem,msize size);
		allocator_(void* mem,msize size,msize poolSize);
		allocator_(stack_alloc* stackAlloc);

		void* alloc_mem(msize size=0);
		void free_mem(void* ptr,msize size=0);
		void clear();
		msize get_size();
		msize get_used();
		msize get_available();
		alloc_t get_type();
		bool32 owns(void* ptr,msize size);
		stack_state save_state();
		void restore_state(stack_state state);
		void clear_buff();

		template <typename T>
		T* alloc(msize count=0)
		{
			return (T*)alloc_mem(sizeof(T)*count);
		}

		template <typename T>
		void free(T* ptr,msize count=0)
		{
			return free_mem(ptr,sizeof(T)*count);
		}

		~allocator_();
	};
}

#endif
