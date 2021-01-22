
namespace dggt::coll
{
	template <uint32 S,typename T>
	starray<S,T> create_starray()
	{
		return starray<S,T>{};
	}

	template <uint32 S,typename T>
	bool32 push(starray<S,T>* arr)
	{
		bool32 result=FALSE;
		if (arr&&S<arr->count)
		{
			++arr->count;
			result=TRUE;
		}
		return result;
	}

	template <uint32 S,typename T>
	bool32 push(starray<S,T>* arr, T* item)
	{
		bool32 result=push(arr);
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

	template <uint32 S,typename T>
	void pop(starray<S,T>* arr)
	{
		if (arr&&arr->count)
		{
			--arr->count;
		}
	}

	template <uint32 S,typename T>
	float32 load_factor(starray<S,T>* arr)
	{
		float32 result=0.0f;
		if (arr)
		{
			result=(float32)arr->count/S;
		}
		return result;
	}

	template <typename S,typename T>
	starray<S,T>::iter(T* data,uint32 current)
	{
		if (data)
		{
			d=data;
			currentIndex=current;
		}
	}

	template <typename S,typename T>
	starray<S,T>::iter()
		:iter(0,0) { }

	template <uint32 S,typename T>
	bool32 starray<S,T>::iter::is_end()
	{
		return currentIndex>S;
	}

	template <uint32 S,typename T>
	bool32 starray<S,T>::iter::is_begin()
	{
		return currentIndex==0;
	}

	template <uint32 S,typename T>
	bool32 starray<S,T>::iter::next()
	{
		bool32 result=FALSE;
		if (!is_end())
		{
			++currentIndex;
			result=TRUE;
		}
		return result;
	}

	template <uint32 S,typename T>
	bool32 starray<S,T>::iter::prev()
	{
		bool32 result=FALSE;
		if (!is_end())
		{
			--currentIndex;
			result=TRUE;
		}
		return result;
	}

	template <uint32 S,typename T>
	T* starray<S,T>::iter::get_item()
	{
		T* result=0;
		if (!is_end())
		{
			return d+currentIndex;
		}
		return result;
	}

	template <uint32 S,typename T>
	const T* starray<S,T>::iter::get_item() const
	{
		return get_item();
	}

	template <uint32 S,typename T>
	T& starray<S,T>::iter::operator*()
	{
		return *get_item();
	}

	template <uint32 S,typename T>
	const T& starray<S,T>::iter::operator*() const
	{
		return *get_item();
	}

	template <uint32 S,typename T>
	starray<S,T>::iter& starray<S,T>::iter::operator++()
	{
		next();
		return *this;
	}

	template <uint32 S,typename T>
	const starray<S,T>::iter& starray<S,T>::iter::operator++(int)
	{
		starray<S,T>::iter& result=*this;
		next();
		return result;
	}

	template <uint32 S,typename T>
	starray<S,T>::iter& starray<S,T>::iter::operator--()
	{
		prev();
		return *this;
	}

	template <uint32 S,typename T>
	const starray<S,T>::iter& starray<S,T>::iter::operator--(int)
	{
		starray<S,T>::iter& result=*this;
		prev();
		return result;
	}

	template <uint32 S,typename T>
	starray<T>::iter begin_iter(starray<S,T>* arr)
	{
		starray::iter result=starray<T>::iter();
		if (arr)
		{
			result=starray<T>::iter(arr,0);
		}
		return result;
	}

	template <uint32 S,typename T>
	starray<T>::iter end_iter(starray<S,T>* arr)
	{
		starray::iter result=starray<T>::iter();
		if (arr)
		{
			result=starray<T>::iter(arr,arr->count-1);
		}
		return result;
	}
}
