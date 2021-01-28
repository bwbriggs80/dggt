namespace
{
	using namespace dggt::mem;

	template <size_t S>
	void* linear_stalloc_mem(stallocator<S>* alloc,size_t size)
	{
		void* result=0;
		if (size&&alloc->get_available()>=size)
		{
			void* data=(void*)alloc->data;
			size_t used=alloc->used;
			if ((void*)data&&alloc->get_available())
			{
				result=ptr_add((void*)data,used);
				alloc->used+=size;
			}
		}
		return result;
	}

	template <size_t S>
	void* stack_stalloc_mem(stallocator<S>* alloc,size_t size)
	{
		return linear_stalloc_mem(alloc,size);
	}

	template <size_t S>
	void* pool_stalloc_mem(stallocator<S>* alloc,size_t size)
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

	template <size_t S>
	void* free_block_stalloc_mem(stallocator<S>* alloc,size_t size)
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
			size_t diff=ABS(bestFit-currentSize);
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
			size_t blockDiff=ABS(bestFitPtr->size-size);
			if (blockDiff>=sizeof(free_block)) // split
			{
				free_block* newBlock=(free_block*)ptr_add(bestFitPtr,size);
				newBlock->size=blockDiff;
				newBlock->next=bestFitPtr->next;
				if (bestFitPtr==alloc->freeHead)
				{
					alloc->freeHead=newBlock;
				}
				else if (prev)
				{
					prev->next=newBlock;
				}
			}
			else // no split
			{
				size=bestFitPtr->size;
				if (bestFitPtr==alloc->freeHead)
				{
					alloc->freeHead=bestFitPtr->next;
				}
				else if (prev)
				{
					prev->next=bestFitPtr->next;
				}
			}
			alloc->used+=size;
			result=bestFitPtr;
		}
		return result;
	}

	template <size_t S>
	void pool_stalloc_free_mem(stallocator<S>* alloc,void* ptr)
	{
		if (alloc->owns(ptr,alloc->pSize))
		{
			pool_block* newBlock=(pool_block*)ptr;
			newBlock->next=alloc->poolHead;
			alloc->poolHead=newBlock;
			alloc->used-=alloc->pSize;
		}
	}

	template <size_t S>
	void free_block_free_mem(stallocator<S>* alloc,void* ptr,size_t size)
	{
		free_block* current=alloc->freeHead;
		free_block* prev=0;
		free_block* blockToFree=(free_block*)ptr;
		if (size<sizeof(free_block))
		{
			size=sizeof(free_block);
		}

		// adjust size for end of stallocator<S> memory considerations.
		void* allocBeg=(void*)alloc->data;
		void* ptrEnd=ptr_add(ptr,size);
		size_t totalSize=addr_diff(allocBeg,ptrEnd);
		size_t endDiff=S-totalSize;
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
	template <size_t S>
	stallocator<S>::stallocator(alloc_t allocType)
		: type(allocType),used(0),state(0)
	{
		switch (type)
		{
			case FREE_BLOCK_ALLOC:
				{
					freeHead=(free_block*)data;
					freeHead->next=0;
				} break;
		}
	}

	template <size_t S>
	stallocator<S>::stallocator(size_t poolSize)
		: stallocator(POOL_ALLOC)
	{
		pSize=poolSize;
		if (pSize>0)
		{
			size_t blockCount=size/pSize;
			for (int i=0;i<blockCount;++i)
			{
				pool_block* newBlock=
					(pool_block*)ptr_add(
							(void*)data,i*pSize);
				newBlock->next=poolHead;
				poolHead=newBlock;
			}
		}
	}

	template <size_t S>
	void* stallocator<S>::alloc_mem(size_t size)
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

	template <size_t S>
	void stallocator<S>::free_mem(void* ptr,size_t size)
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

	template <size_t S>
	void stallocator<S>::clear()
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
						size_t blockCount=S/pSize;
						for (int i=0;i<blockCount;++i)
						{
							pool_block* newBlock=
								(pool_block*)ptr_add(
										(void*)data,i*pSize);
							newBlock->next=poolHead;
							poolHead=newBlock;
						}
					}
				} break;
		}
	}

	template <size_t S>
	size_t stallocator<S>::get_size()
	{
		return S;
	}

	template <size_t S>
	size_t stallocator<S>::get_used()
	{
		return used;
	}

	template <size_t S>
	size_t stallocator<S>::get_available()
	{
		return S-used;
	}

	template <size_t S>
	alloc_t stallocator<S>::get_type()
	{
		return type;
	}

	template <size_t S>
	bool32 stallocator<S>::owns(void* ptr,size_t size)
	{
		return ptr>=(void*)data&&
			ptr_add(ptr,size)<=ptr_add((void*)data,S);
	}

	template <size_t S>
	stack_state stallocator<S>::save_state()
	{
		stack_state result=0;
		if (type==STACK_ALLOC)
		{
			result=used;
			state=used;
		}
		return result;
	}

	template <size_t S>
	void stallocator<S>::restore_state(stack_state state)
	{
		if (type==STACK_ALLOC)
		{
			used=state;
		}
	}

	template <size_t S>
	void stallocator<S>::clear_buff()
	{
		used=0;
		switch (type)
		{
			case STACK_ALLOC:
				{
					state=0;

				} break;
			case POOL_ALLOC:
				{
					pSize=0;
					poolHead=0;
				} break;
			case FREE_BLOCK_ALLOC:
				{
					freeHead=0;
				} break;
		}
	}
}
