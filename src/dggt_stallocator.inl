namespace
{
	using namespace dggt;
	using namespace dggt::mem;

	template <msize Size>
	void* linear_stalloc_mem(allocator_<Size>* alloc,msize size)
	{
		void* result=0;
		if (size&&alloc->get_available()>=size)
		{
			void* data=(void*)alloc->data;
			msize used=alloc->used;
			if ((void*)data&&alloc->get_available())
			{
				result=ptr_add((void*)data,used);
				alloc->used+=size;
			}
		}
		return result;
	}

	template <msize Size>
	void* stack_stalloc_mem(allocator_<Size>* alloc,msize size)
	{
		return linear_stalloc_mem(alloc,size);
	}

	template <msize Size>
	void* pool_stalloc_mem(allocator_<Size>* alloc,msize size)
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

	template <msize Size>
	void* free_block_stalloc_mem(allocator_<Size>* alloc,msize size)
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
		msize bestFit=MAX_SIZE;
		msize minDiff=bestFit;

		while (current)
		{
			msize currentSize=current->size;
			msize diff=ABS(bestFit-currentSize);
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
			msize blockDiff=ABS(bestFitPtr->size-size);
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

	template <msize Size>
	void pool_stalloc_free_mem(allocator_<Size>* alloc,void* ptr)
	{
		if (alloc->owns(ptr,alloc->pSize))
		{
			pool_block* newBlock=(pool_block*)ptr;
			newBlock->next=alloc->poolHead;
			alloc->poolHead=newBlock;
			alloc->used-=alloc->pSize;
		}
	}

	template <msize Size>
	void free_block_free_mem(allocator_<Size>* alloc,void* ptr,msize size)
	{
		free_block* current=alloc->freeHead;
		free_block* prev=0;
		free_block* blockToFree=(free_block*)ptr;
		if (size<sizeof(free_block))
		{
			size=sizeof(free_block);
		}

		// adjust size for end of allocator_<Size> memory considerations.
		void* allocBeg=(void*)alloc->data;
		void* ptrEnd=ptr_add(ptr,size);
		msize totalSize=addr_diff(allocBeg,ptrEnd);
		msize endDiff=Size-totalSize;
		if (endDiff<sizeof(free_block))
		{
			size+=endDiff;
		}

		msize freedSize=size;
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
						msize diff=addr_diff(
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
	template <msize Size>
	allocator_<Size>::allocator_(alloc_t allocType)
		: type(allocType),used(0),state(0)
	{
		ASSERT(allocType!=AUTOSTACK_ALLOC&&allocType!=POOL_ALLOC);
		switch (type)
		{
			case FREE_BLOCK_ALLOC:
				{
					freeHead=(free_block*)data;
					freeHead->next=0;
				} break;
		}
	}

	template <msize Size>
	allocator_<Size>::allocator_(msize poolSize)
		: allocator_(POOL_ALLOC)
	{
		pSize=poolSize;
		if (pSize>0)
		{
			msize blockCount=Size/pSize;
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

	template <msize Size>
	void* allocator_<Size>::alloc_mem(msize size)
	{
		void* result=0;
		switch (type)
		{
			case LINEAR_ALLOC:
				{
					result=::linear_stalloc_mem(this,size);
				} break;
			case STACK_ALLOC:
				{
					result=::stack_stalloc_mem(this,size);
				} break;
			case POOL_ALLOC:
				{
					result=::pool_stalloc_mem(this,size);
				} break;
			case FREE_BLOCK_ALLOC:
				{
					result=::free_block_stalloc_mem(this,size);
				} break;
		}
		return result;
	}

	template <msize Size>
	void allocator_<Size>::free_mem(void* ptr,msize size)
	{
		switch (type)
		{
			case POOL_ALLOC:
				{
					::pool_stalloc_free_mem(this,ptr);
				} break;
			case FREE_BLOCK_ALLOC:
				{
					::free_block_free_mem(this,ptr,size);
				} break;
		}
	}

	template <msize Size>
	void allocator_<Size>::clear()
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
						msize blockCount=Size/pSize;
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

	template <msize Size>
	msize allocator_<Size>::get_size()
	{
		return Size;
	}

	template <msize Size>
	msize allocator_<Size>::get_used()
	{
		return used;
	}

	template <msize Size>
	msize allocator_<Size>::get_available()
	{
		return Size-used;
	}

	template <msize Size>
	alloc_t allocator_<Size>::get_type()
	{
		return type;
	}

	template <msize Size>
	bool32 allocator_<Size>::owns(void* ptr,msize size)
	{
		return ptr>=(void*)data&&
			ptr_add(ptr,size)<=ptr_add((void*)data,Size);
	}

	template <msize Size>
	stack_state allocator_<Size>::save_state()
	{
		stack_state result=0;
		if (type==STACK_ALLOC)
		{
			result=used;
			state=used;
		}
		return result;
	}

	template <msize Size>
	void allocator_<Size>::restore_state(stack_state state)
	{
		if (type==STACK_ALLOC)
		{
			used=state;
		}
	}

	template <msize Size>
	void allocator_<Size>::clear_buff()
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
