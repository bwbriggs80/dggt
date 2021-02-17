
namespace dggt::coll
{
	template <typename T>
	typename darray<T>::iter darray<T>::operator[](uint32 index)
	{
		typename darray<T>::iter result=typename darray<T>::iter();
		if (index<count)
		{
			result.d=data;
			result.current=index;
			result.end=index;
		}
		return result;
	}

	template <typename T>
	const typename darray<T>::iter darray<T>::operator[](uint32 index) const
	{
		return operator[](index);
	}

	template <typename T,typename Alloc>
	darray<T> create_darray(Alloc* alloc,uint32 size)
	{
		darray<T> result={};
		if (alloc&&size)
		{
			T* data=alloc->template alloc<T>(size);
			if (data)
			{
				result.data=data;
				result.size=size;
			}
		}
		return result;
	}

	template <typename T,typename Alloc>
	typename darray<T>::iter reserve(darray<T>* arr,uint32 count,
			uint32 buffer,Alloc* alloc)
	{
		typename darray<T>::iter result=typename darray<T>::iter();
		if (arr)
		{
			uint32 newCount=arr->count+count;
			uint32 oldCount=arr->count;

			if (newCount>=arr->size&&alloc)
			{
				ASSERT(buffer>newCount);
				//TODO: figure out number of doublings it would be
				//TODO: as if we were calling consecutive push calls.
				result=resize(arr,buffer,alloc);
			}

			if (result.is_valid())
			{
				arr->count=newCount;
				result.current=oldCount;
				result.end=newCount-1;
				result.size=arr->count;
			}
		}
		return result;
	}


	template <typename T,typename Alloc>
	typename darray<T>::iter resize(
			darray<T>* arr,uint32 newSize,Alloc* alloc)
	{
		typename darray<T>::iter result=typename darray<T>::iter();
		if (arr&&alloc&&alloc->owns(arr->data)&&
				newSize!=arr->size)
		{
			uint32 oldSize=arr->size;
			T* oldData=arr->data;
			T* newData=template alloc->alloc<T>(newSize);
			if (newData)
			{
				uint32 maxSize=MAX(oldSize,newSize);
				for (uint32 i=0;i<maxSize;++i)
				{
					newData[i]=oldData[i];
				}
				alloc->free(oldData,oldSize);
				arr->size=newSize;
				arr->data=newData;
				result=typename darray<T>::iter(arr->data,0,arr->count);
			}
		}
		return result;
	}


	template <typename T,typename Alloc>
	typename darray<T>::iter push(darray<T>* arr,Alloc* alloc)
	{
		typename darray<T>::iter result=typename darray<T>::iter();
		if (arr)
		{
			result=reserve(arr,1,2*arr->size,alloc);
		}
		return result;
	}

	template <typename T,typename Alloc>
	typename darray<T>::iter push(darray<T>* arr,T item,Alloc* alloc)
	{
		typename darray<T>::iter result=push(arr,alloc);
		if (result.is_valid())
		{
			arr->data[arr->count-1]=item;
		}
		return result;
	}

	template <typename T,typename Alloc>
	typename darray<T>::iter pop(darray<T>* arr,Alloc* alloc)
	{
		typename darray<T>::iter result=typename darray<T>::iter();
		if (arr&&arr->count)
		{
			--arr->count;
			if (alloc&&
					alloc->owns(arr->data,arr->size)&&
					load_factor(arr)<0.25f)
			{
				resize(arr,oldSize/2,alloc);
			}
			result=typename darray<T>::iter(
					arr->data,arr->count-1,arr->count);
		}
		return result;
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
	darray<T>::iter::iter(T* data,uint32 currentIndex,
			uint32 endIndex,uint32 collSize)
	:d(data),current(currentIndex),end(endIndex),size(collSize) { }

	template <typename T>
	darray<T>::iter::iter(T* data,uint32 currentIndex,uint32 collSize)
	:iter(data,currentIndex,collSize-1,collSize) { }

	template <typename T>
	darray<T>::iter::iter()
		:iter(0,0,0) { }

	template <typename T>
	bool32 darray<T>::iter::is_end()
	{
		return current>=end;
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
	bool32 darray<T>::iter::is_valid()
	{
		return d;
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
	typename darray<T>::iter& darray<T>::iter::operator++()
	{
		next();
		return *this;
	}

	template <typename T>
	const typename darray<T>::iter& darray<T>::iter::operator++(int)
	{
		typename darray<T>::iter& result=*this;
		next();
		return result;
	}

	template <typename T>
	typename darray<T>::iter& darray<T>::iter::operator--()
	{
		prev();
		return *this;
	}

	template <typename T>
	const typename darray<T>::iter& darray<T>::iter::operator--(int)
	{
		typename darray<T>::iter& result=*this;
		prev();
		return *this;
	}

	template <typename T>
	typename darray<T>::iter begin_iter(darray<T>* arr)
	{
		typename darray<T>::iter result=typename darray<T>::iter();
		if (arr)
		{
			result=typename darray<T>::iter(arr->data,0,arr->count);
		}
		return result;
	}

	template <typename T>
	typename darray<T>::iter end_iter(darray<T>* arr)
	{
		typename darray<T>::iter result=typename darray<T>::iter();
		if (arr)
		{
			result=typename darray<T>::iter(arr->data,arr->count-1,arr->count);
		}
		return result;
	}

	template <typename T>
	typename darray<T>::iter sub_iter(darray<T>* arr,
			uint32 begin,uint32 end)
	{
		ASSERT(begin<=end);
		typename darray<T>::iter result=typename darray<T>::iter();
		if (arr)
		{
			if (end>arr->count)
			{
				end=size;
			}
			result=typename darray<T>::iter(arr->data,begin,end);
		}
		return result;
	}

	template <typename T>
	uint32 get_size(darray<T>* arr)
	{
		return arr->size;
	}

	template <typename T>
	uint32 get_count(darray<T>* arr)
	{
		return arr->count;
	}


}
