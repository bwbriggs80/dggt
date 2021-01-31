#ifndef _DGGT_STALLOCATOR_H_
#define _DGGT_STALLOCATOR_H_

#include "dggt_defs.h"
#include "dggt_allocator.h"

namespace dggt::mem
{
	template <msize Size>
	struct allocator_
	{
		alloc_t type;
		ubyte data[Size];
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
		};


		allocator_(alloc_t allocType);
		allocator_(msize poolSize);

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
	};

	template <msize Size>
	using stallocator=allocator_<Size>;
}

#include "dggt_stallocator.inl"

#endif
