#ifndef _DGGT_STALLOCATOR_H_
#define _DGGT_STALLOCATOR_H_

#include "dggt_defs.h"
#include "dggt_allocator.h"

namespace dggt::mem
{
	template <msize Size>
	struct allocator_
	{
		ubyte data[Size];
		allocator childAlloc;

		allocator_(alloc_t allocType)
			: childAlloc(allocator_(allocType,(void*)data,Size)) { }
		allocator_(msize poolSize)
			: childAlloc(allocator_((void*)data,Size,poolSize)) { }
		allocator_(stack_alloc* stackAlloc)
			: childAlloc(allocator_(stackAlloc)) { }
		allocator_(autostack_alloc& autostackAlloc)
			: childAlloc(allocator_(autostackAlloc) { }

		void* alloc_mem(msize size=0) { return childAlloc.alloc_mem(size); }
		void free_mem(void* ptr,msize size=0) { childAlloc.free_mem(ptr,size); }
		void clear() { childAlloc.clear(); }
		msize get_size() { return Size; }
		msize get_used() { return childAlloc.get_used(); }
		msize get_available() { return childAlloc.get_available(); }
		alloc_t get_type() { return childAlloc.get_type(); }
		bool32 owns(void* ptr,msize size) { return childAlloc.owns(ptr,size); }
		stack_state save_state() { return childAlloc.save_state(); }
		void restore_state(stack_state state) { childAlloc.restore_state(state); }
		void clear_buff() { childAlloc.clear_buff(); }

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

#endif
