
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
}
