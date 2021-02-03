
namespace dggt::coll
{
	template <typename T,typename Alloc>
	sllist<T> create_sllist(Alloc* alloc=0,uint32 size=0)
	{
		sllist<T> result={};
		if (alloc&&size)
		{
			reserve(&result,size,alloc);
		}
		return result;
	}

	template <typename T>
	uint32 get_count(sllst<T>* lst)
	{
		return lst->count;
	}

	template <typename T,typename Alloc>
	bool32 reserve(sllst<T>* lst,uint32 count,Alloc* alloc)
	{
		bool32 result=0;
		if (lst&&alloc&&
				alloc->owns(lst->head)&&count)
		{
			for (uint32 i=0;i<count;++i)
			{
				slnode<T>* newNode=template alloc->alloc<T>(1);
				if (newNode)
				{
					newNode->next=lst->head;
					lst->head=newNode;
					++result;
					++lst->count;
				}
				else
				{
					break;
				}
			}
		}
		return result;
	}

	template <typename T,typename Alloc>
	bool32 push(sllst<T>* lst,Alloc* alloc)
	{
		bool32 result=reserve(lst,1,alloc);
		return result;
	}

	template <typename T,typename Alloc>
	bool32 push(sllst<T>* lst,T item,Alloc* alloc)
	{
		bool32 result=push(lst,alloc);
		if (result)
		{
			lst->head->item=item;
		}
		return result;
	}

	template <typename T,typename Alloc>
	void pop(sllst<T>* lst,Alloc* alloc)
	{
		if (lst&&alloc&&alloc->count&&
				alloc->owns(lst->head))
		{
			slnode<T>* head=lst->head;
			lst->head=lst->head->next;
			--lst->count;
		}
	}

	template <typename T>
	typename sllst<T>::iter begin_iter(sllst<T>* lst)
	{
		return iter(current);
	}
}
