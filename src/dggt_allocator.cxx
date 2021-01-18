#include "dggt_allocator.h"

namespace
{
	using namespace dggt::mem;
	void* linear_alloc_mem(allocator* alloc,size_t size)
	{
		void* result=0;
		if (size&&alloc->get_available()>=size)
		{
			void* memAddr=alloc->memAddr;
			size_t used=alloc->used;
			if (alloc->memAddr&&alloc->get_available())
			{
				result=ptr_add(memAddr,used);
				alloc->used+=size;
			}
		}
		return result;
	}

	void* stack_alloc_mem(allocator* alloc,size_t size)
	{
		return linear_alloc_mem(alloc,size);
	}

	void* pool_alloc_mem(allocator* alloc,size_t size)
	{
		void* result=0;
		if (alloc->get_available()>=alloc->pSize)
		{
			result=alloc->poolHead;
			alloc->poolHead=alloc->poolHead->next;
			alloc->used+=alloc->pSize;
		}
		return result;
	}

	void* free_block_alloc_mem(allocator* alloc,size_t size)
	{
		free_block* result=0;
		if (alloc->get_available()<size)
		{
			return result;
		}

		if (size<sizeof(free_block))
		{
			size=sizeof(free_block);
		}

		
		free_block* current=alloc->freeHead;
		free_block* prev=0;
		free_block* bestFitPtr=0;
		size_t bestFit=MAX_SIZE;
		size_t minDiff=bestFit;

		while (current)
		{
			size_t currentSize=current->size;
			size_t diff=ABS(currentSize-bestFit);
			if (diff<minDiff)
			{
				minDiff=diff;
				bestFit=currentSize;
				bestFitPtr=current;
			}
			prev=current;
			current=current->next;
		}

		if (bestFitPtr)
		{
			size_t blockDiff=ABS(size-bestFitPtr->size);
			if (blockDiff>=sizeof(free_block)) // split
			{
				free_block* newBlock=(free_block*)ptr_add(bestFitPtr,size);
				newBlock->size=blockDiff;
				newBlock->next=bestFitPtr->next;
				if (prev)
				{
					prev->next=newBlock;
				}
			}
			else // no split
			{
				size=bestFitPtr->size;
				if (prev)
				{
					prev->next=bestFitPtr->next;
				}
			}
			alloc->used+=size;
			result=bestFitPtr;

		}
		return result;
	}

	void pool_alloc_free_mem(allocator* alloc,void* ptr)
	{
		if (alloc->owns(ptr,alloc->pSize))
		{
			pool_block* newBlock=(pool_block*)ptr;
			newBlock->next=alloc->poolHead;
			alloc->poolHead=newBlock;
			alloc->used-=alloc->pSize;
		}
	}

	void free_block_free_mem(allocator* alloc,void* ptr,size_t size)
	{
		free_block* current=alloc->freeHead;
		free_block* prev=0;
		free_block* blockToFree=(free_block*)ptr;
		if (size<sizeof(free_block))
		{
			size=sizeof(free_block);
		}
		size_t freedSize=size;
		if (alloc->owns(blockToFree,size))
		{
			while (current<blockToFree)
			{
				prev=current;
				current=current->next;
			}
			free_block* newBlock=blockToFree;
			if (prev)
			{
				if (ptr_add(prev,prev->size)==newBlock) // merge right
				{
					prev->size=prev->size+newBlock->size;
					newBlock=prev;
					size=newBlock->size;
					newBlock->next=current;
				}
				else // no merge right
				{
					prev->next=newBlock;
				}
				if (current)
				{
					if (ptr_add(newBlock,size)==current) // merge left
					{
						newBlock->size=newBlock->size+current->size;
						newBlock->next=current->next;
					}
					else // no merge left
					{
						newBlock->next=current;
					}
				}
			}
			else if (current)
			{
				if (ptr_add(newBlock,size)==current) // merge left
				{
					newBlock->size=newBlock->size+current->size;
					newBlock->next=current->next;
				}
				else // no merge left
				{
					newBlock->next=current;
				}
				alloc->freeHead=newBlock;
			}
			else
			{
				alloc->freeHead=newBlock;
			}
			alloc->used-=freedSize;
		}
	}
}

namespace dggt::mem
{
	void* allocator::alloc_mem(size_t size)
	{
		void* result=0;
		switch (type)
		{
			case LINEAR_ALLOC:
				{
					result=::linear_alloc_mem(this,size);
				} break;
			case STACK_ALLOC:
				{
					result=::stack_alloc_mem(this,size);
				} break;
			case POOL_ALLOC:
				{
					result=::pool_alloc_mem(this,size);
				} break;
			case FREE_BLOCK_ALLOC:
				{
					result=::free_block_alloc_mem(this,size);
				} break;
		}
		return result;
	}

	void allocator::free_mem(void* ptr,size_t size)
	{
		switch (type)
		{
			case POOL_ALLOC:
				{
					::pool_alloc_free_mem(this,ptr);
				} break;
			case FREE_BLOCK_ALLOC:
				{
					::free_block_free_mem(this,ptr,size);
				} break;
		}
	}

	void allocator::clear()
	{
		switch (type)
		{
			case LINEAR_ALLOC:
				{
					used=0;
				} break;
			case STACK_ALLOC:
				{
					used=0;
					state=0;
				} break;
			case POOL_ALLOC:
				{
					used=0;
					if (pSize>0)
					{
						size_t blockCount=memSize/pSize;
						for (int i=0;i<blockCount;++i)
						{
							pool_block* newBlock=
								(pool_block*)ptr_add(
										memAddr,i*pSize);
							newBlock->next=poolHead;
							poolHead=newBlock;
						}
					}
				} break;
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

	bool32 allocator::owns(void* ptr,size_t size)
	{
		return ptr>=memAddr&&
			ptr_add(ptr,size)<=ptr_add(memAddr,memSize);
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
