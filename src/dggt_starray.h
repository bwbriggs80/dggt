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

		struct iter
		{
			T* d;
			uint32 currentIndex;

			iter(T* data,uint32 current);
			iter();

			bool32 is_end();
			bool32 is_begin();
			bool32 next();
			bool32 prev();
			T* get_item();
			const T* get_item() const;

			T& operator*();
			const T& operator*() const;

			iter& operator++();
			const iter& operator++(int);
			iter& operator--();
			const iter& operator--(int);
		};
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

	template <uint32 S,typename T>
	starray<T>::iter begin_iter(starray<S,T>* arr);

	template <uint32 S,typename T>
	starray<T>::iter end_iter(starray<S,T>* arr);
}

#include "dggt_starray.inl"

#endif
