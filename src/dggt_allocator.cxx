#include "dggt_allocator.h"

namespace
{
	using namespace dggt::mem;
	using namespace dggt;

	void* linear_alloc_mem(allocator_<0>* alloc,size_t size)
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

	void* stack_alloc_mem(allocator_<0>* alloc,size_t size)
	{
		return linear_alloc_mem(alloc,size);
	}

	void* pool_alloc_mem(allocator_<0>* alloc,size_t size)
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

	void* free_block_alloc_mem(allocator_<0>* alloc,size_t size)
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
		free_block* bestFitPrev=0;
		size_t bestFit=MAX_SIZE;
		size_t minDiff=bestFit;

		while (current)
		{
			size_t currentSize=current->size;
			size_t diff=MAX(bestFit,currentSize)-MIN(bestFit,currentSize);
			if (diff<minDiff)
			{
				minDiff=diff;
				bestFit=currentSize;
				bestFitPrev=prev;
				bestFitPtr=current;
			}
			prev=current;
			current=current->next;
		}

		if (bestFitPtr)
		{
			size_t blockDiff=MAX(bestFit,size)-MIN(bestFit,size);
			if (blockDiff>=sizeof(free_block)) // split
			{
				free_block* newBlock=(free_block*)ptr_add(bestFitPtr,size);
				newBlock->size=blockDiff;
				newBlock->next=bestFitPtr->next;
				if (bestFitPtr==alloc->freeHead)
				{
					alloc->freeHead=newBlock;
				}
				else if (bestFitPrev)
				{
					bestFitPrev->next=newBlock;
				}
			}
			else // no split
			{
				size=bestFitPtr->size;
				if (bestFitPtr==alloc->freeHead)
				{
					alloc->freeHead=bestFitPtr->next;
				}
				else if (bestFitPrev)
				{
					bestFitPrev->next=bestFitPtr->next;
				}
			}
			alloc->used+=size;
			result=bestFitPtr;
		}
		return result;
	}

	void pool_alloc_free_mem(allocator_<0>* alloc,void* ptr)
	{
		if (alloc->owns(ptr,alloc->pSize))
		{
			pool_block* newBlock=(pool_block*)ptr;
			newBlock->next=alloc->poolHead;
			alloc->poolHead=newBlock;
			alloc->used-=alloc->pSize;
		}
	}

	void free_block_free_mem(allocator_<0>* alloc,void* ptr,size_t size)
	{
		if (alloc->type!=POOL_ALLOC)
		{
			ASSERT(size)
		}
		free_block* current=alloc->freeHead;
		free_block* prev=0;
		free_block* blockToFree=(free_block*)ptr;
		if (size<sizeof(free_block))
		{
			size=sizeof(free_block);
		}

		// adjust size for end of allocator memory considerations.
		void* allocBeg=alloc->memAddr;
		void* ptrEnd=ptr_add(ptr,size);
		size_t totalSize=addr_diff(allocBeg,ptrEnd);
		size_t endDiff=alloc->memSize-totalSize;
		if (endDiff<sizeof(free_block))
		{
			size+=endDiff;
		}

		size_t freedSize=size;
		if (alloc->owns(blockToFree,size))
		{
			while (current&&current<blockToFree)
			{
				prev=current;
				current=current->next;
			}
			free_block* newBlock=blockToFree;
			newBlock->size=size;

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
					else if (
							addr_diff(
								ptr_add(newBlock,size),current)<sizeof(free_block)) // adjust and merge left
					{
						size_t diff=addr_diff(
								ptr_add(newBlock,size),current);
						newBlock->size=diff+newBlock->size+current->size;
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
	allocator_<0>::allocator_(alloc_t allocType,void* mem,size_t size)
		: type(allocType),memAddr(mem),
		memSize(size),used(0),state(0)
	{
		ASSERT(allocType!=POOL_ALLOC);
		if (type==FREE_BLOCK_ALLOC)
		{
			freeHead=(free_block*)mem;
			freeHead->size=memSize;
			freeHead->next=0;
		}
	}

	allocator_<0>::allocator_(void* mem,size_t size,size_t poolSize)
		: allocator_(POOL_ALLOC,mem,size)
	{
		pSize=poolSize;
		if (pSize>0)
		{
			size_t blockCount=size/pSize;
			for (uint32 i=0;i<blockCount;++i)
			{
				pool_block* newBlock=
					(pool_block*)ptr_add(
							mem,i*pSize);
				newBlock->next=poolHead;
				poolHead=newBlock;
			}
		}
	}

	allocator_<0>::allocator_(stack_alloc* stackAlloc)
		: allocator_(AUTOSTACK_ALLOC,0,0)
	{
		ASSERT(stackAlloc);
		stkAlloc=stackAlloc;
		state=stkAlloc->save_state();
	}

	allocator_<0>::allocator_(const autostack_alloc& autostackAlloc)
		: allocator_(AUTOSTACK_ALLOC,0,0)
	{
		stkAlloc=autostackAlloc.stkAlloc;
		state=stkAlloc->save_state();
	}

	void* allocator_<0>::alloc_mem(size_t size)
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
			case AUTOSTACK_ALLOC:
				{
					result=::stack_alloc_mem(stkAlloc,size);
				} break;
		}
		return result;
	}

	void allocator_<0>::free_mem(void* ptr,size_t size)
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
			default:
				{
					ASSERT(0);
				} break;
		}
	}

	void allocator_<0>::clear()
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
						for (uint32 i=0;i<blockCount;++i)
						{
							pool_block* newBlock=
								(pool_block*)ptr_add(
										memAddr,i*pSize);
							newBlock->next=poolHead;
							poolHead=newBlock;
						}
					}
				} break;
			case FREE_BLOCK_ALLOC:
				{
					used=0;
					state=0;
					freeHead=(free_block*)memAddr;
					freeHead->next=0;
					freeHead->size=memSize;
				} break;
			case AUTOSTACK_ALLOC:
				{
					stkAlloc->clear();
				} break;
		}
	}

	size_t allocator_<0>::get_size()
	{
		return memSize;
	}

	size_t allocator_<0>::get_used()
	{
		return used;
	}

	size_t allocator_<0>::get_available()
	{
		return memSize-used;
	}

	alloc_t allocator_<0>::get_type()
	{
		return type;
	}

	bool32 allocator_<0>::owns(void* ptr,size_t size)
	{
		return ptr>=memAddr&&
			ptr_add(ptr,size)<=ptr_add(memAddr,memSize);
	}

	stack_state allocator_<0>::save_state()
	{
		stack_state result=0;
		if (type==STACK_ALLOC)
		{
			result=used;
			state=used;
		}
		return result;
	}

	void allocator_<0>::restore_state(stack_state state)
	{
		if (type==STACK_ALLOC)
		{
			used=state;
		}
	}

	void allocator_<0>::clear_buff()
	{
		memAddr=0;
		memSize=0;
		used=0;
		if (type==STACK_ALLOC)
		{
			state=0;
		}
		else if (type==AUTOSTACK_ALLOC)
		{
			stkAlloc->clear_buff();
		}
		else if (type==POOL_ALLOC)
		{
			pSize=0;
			poolHead=0;
		}
		else if (type==FREE_BLOCK_ALLOC)
		{
			freeHead=0;
		}
	}

	allocator_<0>::~allocator_()
	{
		if (type==AUTOSTACK_ALLOC)
		{
			stkAlloc->restore_state(state);
		}
	}
}
