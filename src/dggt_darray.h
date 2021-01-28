#ifndef _DGGT_DARRAY_H_
#define _DGGT_DARRAY_H_

#include "dggt_allocator.h"
#include "dggt_defs.h"

using namespace dggt::mem;

namespace dggt::coll
{
	template <typename T>
	struct darray
	{
		T* data;
		uint32 count;
		uint32 size;

		T& operator[](uint32 index) { return data[index]; }
		const T& operator[](uint32 index) const { return data[index]; }

		struct iter
		{
			T* d;
			uint32 current;
			uint32 size;

			iter(T* data,uint32 currentIndex,uint32 collSize);
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

	template <typename T>
	uint32 get_size(darray<T>* arr);

	template <typename T>
	uint32 get_count(darray<T>* arr);

	template <typename T>
	darray<T> create_darray(allocator* alloc,uint32 size=2);

	template <typename T>
	bool32 push(darray<T>* arr,allocator* alloc);

	template <typename T>
	bool32 push(darray<T>* arr,T item,allocator* alloc);

	template <typename T>
	void pop(darray<T>* arr,allocator* alloc);

	template <typename T>
	float32 load_factor(darray<T>* arr);

	template <typename T>
	typename darray<T>::iter begin_iter(darray<T>* arr);

	template <typename T>
	typename darray<T>::iter end_iter(darray<T>* arr);
}

#include "dggt_darray.inl"

#endif
