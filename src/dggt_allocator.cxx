#include "dggt_allocator.h"

namespace dggt::mem
{
	void* allocator::alloc_mem(size_t size)
	{
		void* result=0;
		if (memAddr&&get_available())
		{
			result=ptr_add(memAddr,used);
			used+=size;
		}
		return result;
	}

	void allocator::free_mem(void* ptr,size_t size) { }

	void allocator::clear()
	{
		used=0;
		if (type==STACK_ALLOC)
		{
			state=0;
		}
	}

	size_t allocator::get_size()
	{
		return memSize;
	}

	size_t allocator::get_used()
	{
		return used;
	}

	size_t allocator::get_available()
	{
		return memSize-used;
	}

	alloc_t allocator::get_type()
	{
		return type;
	}

	stack_state allocator::save_state()
	{
		stack_state result=0;
		if (type==STACK_ALLOC)
		{
			result=used;
			state=used;
		}
		return result;
	}

	void allocator::restore_state(stack_state state)
	{
		if (type==STACK_ALLOC)
		{
			used=state;
		}
	}
}
