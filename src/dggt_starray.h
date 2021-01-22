#ifndef _DGGT_STARRAY_H_
#define _DGGT_STARRAY_H_

#include "dggt_mem.h"
#include "dggt_defs.h"

namespace dggt::coll
{
	template <uint32 S,typename T>
	struct starray
	{
		T[S] data;
		uint32 count;

		T& operator[](uint32 index) { return data[index]; }
		const T& operator[](uint32 index) const { return data[index]; }
	};

	template <uint32 S,typename T>
	starray<S,T> create_starray();

	template <uint32 S,typename T>
	bool32 push(starray<S,T>* arr);

	template <uint32 S,typename T>
	bool32 push(starray<S,T>* arr, T* item);

	template <uint32 S,typename T>
	void pop(starray<S,T>* arr);

	template <uint32 S,typename T>
	float32 load_factor(starray<S,T>* arr);
}

#include "dggt_starray.inl"

#endif
