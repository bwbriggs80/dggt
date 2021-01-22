
namespace dggt::coll
{
	template <typename T>
	darray<T> create_darray(allocator* alloc,uint32 size)
	{
		darray<T> result={};
		if (alloc&&size)
		{
			T* data=alloc->alloc<T>(size);
			if (data)
			{
				result.data=data;
				result.size=size;
			}
		}
		return result;
	}

	template <typename T>
	bool32 push(darray<T>* arr,allocator* alloc)
	{
		if (arr)
		{
			bool32 success=TRUE;
			if (count==size&&alloc)
			{
				uint32 oldSize=arr->size;
				uint32 newSize=2*oldSize;
				T* oldData=arr->data;
				T* newData=alloc->alloc<T>(newSize);
				if (newData)
				{
					for (uint32 i=0;i<oldSize;++i)
					{
						newData[i]=oldData[i];
					}
					arr->data=newData;
				}
				else
				{
					success=FALSE;
				}
			}

			if (success)
			{
				++arr->count;
			}
		}
	}

	template <typename T>
	bool32 push(darray<T>* arr,T* item,allocator* alloc)
	{
		bool32 result=push(arr,alloc);
		if (item&&result)
		{
			arr->data[arr->count-1]=*item;
			result=TRUE;
		}
		else
		{
			result=FALSE;
		}
		return result;
	}

	template <typename T>
	void pop(darray<T>* arr,allocator* alloc)
	{
		if (arr&&arr->count)
		{
			--arr->count;
			if (alloc&&
					alloc->owns(arr->data)&&
					load_factor(arr)<0.25f)
			{
				uint32 oldSize=arr->size;
				uint32 newSize=oldSize/2;
				T* oldData=arr->data;
				T* newData=alloc->alloc<T>(newSize);
				for (uint32 i=0;i<newSize;++i)
				{
					newData[i]=oldData[i];
				}
				alloc->free(oldData);
				alloc->data=newData;
			}
		}
	}

	template <typename T>
	float32 load_factor(darray<T>* arr)
	{
		float32 result=0.0f;
		if (arr&&arr->size)
		{
			result=(float32)arr->count/arr->size;
		}
		return result;
	}

	template <typename T>
	darray<T>::iter(T* data,uint32 currentIndex,uint32 collSize)
		:d(data),current(currentSize),size(collSize) { }

	template <typename T>
	darray<T>::iter()
		:iter(0,0,0) { }

	template <typename T>
	bool32 darray<T>::iter::is_end()
	{
		return current>size;
	}

	template <typename T>
	bool32 darray<T>::iter::is_begin()
	{
		return current==0;
	}

	template <typename T>
	bool32 darray<T>::iter::next()
	{
		bool32 result=FALSE;
		if (!is_end())
		{
			++current;
			result=TRUE;
		}
		return result;
	}

	template <typename T>
	bool32 darray<T>::iter::prev()
	{
		bool32 result=FALSE;
		if (!is_end())
		{
			--current;
			result=TRUE;
		}
		return result;
	}

	template <typename T>
	T* darray<T>::iter::get_item()
	{
		T* result=0;
		if (!is_end())
		{
			return d+current;
		}
		return result;
	}

	template <typename T>
	const T* darray<T>::iter::get_item() const
	{
		return get_item();
	}

	template <typename T>
	T& darray<T>::iter::operator*()
	{
		return *get_item();
	}

	template <typename T>
	const T& darray<T>::iter::operator*() const
	{
		return *get_item();
	}

	template <typename T>
	darray<T>::iter& darray<T>::operator++()
	{
		next();
		return *this;
	}

	template <typename T>
	const iter& operator++(int)
	{
		darray<T>::iter& result=*this;
		next();
		return result;
	}

	template <typename T>
	iter& operator--()
	{
		prev();
		return *this;
	}

	template <typename T>
	const iter& operator--(int)
	{
		darray<T>::iter& result=*this;
		prev();
		return *this;
	}

	template <typename T>
	darray<T>::iter begin_iter(darray<T>* arr)
	{
		darray<T>::iter result=darray<T>::iter();
		if (arr)
		{
			result=darray<T>::iter(arr->data,0,arr->size);
		}
		return result;
	}

	template <typename T>
	darray<T>::iter end_iter(darray<T>* arr)
	{
		darray<T>::iter result=darray<T>::iter();
		if (arr)
		{
			result=darray<T>::iter(arr->data,arr->size-1,arr->size);
		}
		return result;
	}
}
