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
		@returns The address that is offset bytes away from ptr.
	  */
	inline void* ptr_add(void* ptr,size_t offset)
	{
		return (void*)((unsigned char*)ptr+offset);
	}

	/**
	  Subtracts an offset to a memory address.
	  @param ptr Pointer to memory.
	  @param offset Offset in bytes to subtract from address.
	  @returns The address that is offset bytes away from ptr.
	  */
	inline void* ptr_sub(void* ptr,size_t offset)
	{
		return (void*)((unsigned char*)ptr-offset);
	}

	/**
	  Calculates how many bytes between addresses.
	  @param lhs The left most memory address.
	  @param rhs The right most memory address.
	  @returns The distance in bytes between addresses.
	  */
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

	/**
	  Allocates memory.
	  @param size Size in bytes of desired memory.
	  @returns Pointer to the beginning of requested memory.
	  */
	void* alloc_mem(size_t size);

	/**
	  Frees memory previously returned by alloc_mem
	  @param ptr Address of memory to free.
	  @param size Size of the memory to free.
	  */
	void free_mem(void* ptr,size_t size);

	/**
	  Templated version of alloc_mem.  Allocates memory of desired type.
	  @tparam T The desired type of memory.
	  @param count The number of T objects that can fit in the memory.
	  @returns Pointer to the beginning of requested.
	  */
	template <typename T>
	T* alloc(size_t count)
	{
		return (T*)alloc_mem(sizeof(T)*count);
	}

	/**
	  Templated version of free_mem.  Frees memory previously allocated by alloc.
	  @tparam T The type of memory being freed.
	  @param ptr Address of memory to free.
	  @param count Size of memory in number of objects of type T.
	  */
	template <typename T>
	void free(T* ptr,size_t count)
	{
		free_mem((void*)ptr,sizeof(T)*count);
	}
}

#endif
