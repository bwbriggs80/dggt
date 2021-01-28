#ifndef _DGGT_STALLOCATOR_H_
#define _DGGT_STALLOCATOR_H_

#include "dggt_defs.h"
#include "dggt_allocator.h"

namespace dggt::mem
{
	template <size_t S>
	struct stallocator
	{
		alloc_t type;
		ubyte data[S];
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


		allocator(alloc_t allocType);
		allocator(size_t poolSize);

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
	};
}

#include "dggt_stallocator.inl"

#endif
