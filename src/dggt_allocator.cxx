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
}
