//TODO: everything is iterators!! 0.o
#ifndef _DGGT_DARRAY_H_
#define _DGGT_DARRAY_H_

#include "dggt_defs.h"

using namespace dggt::mem;

namespace dggt::coll
{
	template <typename T>
	struct darray
	{
		struct iter
		{
			T* d;
			uint32 current;
			uint32 end;
			uint32 size;

			iter(T* data,uint32 currentIndex,uint32 endIndex,uint32 collSize);
			iter(T* data,uint32 currentIndex,uint32 collSize);
			iter();

			bool32 is_end();
			bool32 is_begin();
			bool32 next();
			bool32 prev();
			bool32 is_valid();
			T* get_item();
			const T* get_item() const;

			T& operator*();
			const T& operator*() const;

			iter& operator++();
			const iter& operator++(int);
			iter& operator--();
			const iter& operator--(int);
		};

		T* data;
		uint32 count;
		uint32 size;

		typename darray<T>::iter operator[](uint32 index);
		const typename darray<T>::iter operator[](uint32 index) const;
	};

	template <typename T,typename Alloc>
	darray<T> create_darray(Alloc* alloc,uint32 size=2);

	template <typename T>
	uint32 get_size(darray<T>* arr);

	template <typename T>
	uint32 get_count(darray<T>* arr);

	//TODO: remove buffer param
	template <typename T,typename Alloc>
	typename darray<T>::iter reserve(
			darray<T>* arr,uint32 count,uint32 buffer,Alloc* alloc);

	//TODO: returns begin_iter
	template <typename T,typename Alloc>
	typename darray<T>::iter resize(darray<T>* arr,uint32 newSize,Alloc* alloc);

	template <typename T,typename Alloc>
	typename darray<T>::iter push(darray<T>* arr,Alloc* alloc);

	template <typename T,typename Alloc>
	typename darray<T>::iter push(darray<T>* arr,T item,Alloc* alloc);

	//TODO: return iterator to newly inserted, uninitialized item.
	//WARNING: linear time.
	template <typename T,typename Alloc>
	typename darray<T>::iter insert_after(darray<T>* arr,uint32 prevIndex,
			Alloc* alloc);
	
	//TODO: return iterator to newly inserted, uninitialized item.
	//WARNING: linear time.
	template <typename T,typename Alloc>
	typename darray<T>::iter insert(darray<T>* arr,uint32 prevIndex,
			Alloc* alloc);

	//TODO: return iterator to newly inserted, uninitialized item.
	//WARNING: linear time.
	template <typename T,typename Alloc>
	typename darray<T>::iter insert_before(darray<T>* arr,uint32 nextIndex,
			Alloc* alloc);

	//TODO: return iterator to newly inserted, initialized item.
	//WARNING: linear time.
	template <typename T,typename Alloc>
	typename darray<T>::iter insert_after(darray<T>* arr,uint32 prevIndex,
			T item,Alloc* alloc);

	//TODO: return iterator to newly inserted, initialized item.
	//WARNING: linear time.
	template <typename T,typename Alloc>
	typename darray<T>::iter insert(darray<T>* arr,uint32 prevIndex,
			T item,Alloc* alloc);

	//TODO: return iterator to newly inserted, initialized item.
	//WARNING: linear time.
	template <typename T,typename Alloc>
	typename darray<T>::iter insert_before(darray<T>* arr,uint32 nextIndex,
			T item,Alloc* alloc);

	//TODO: pop should return iterator to new head.
	template <typename T,typename Alloc>
	typename darray<T>::iter pop(darray<T>* arr,Alloc* alloc);

	//TODO: returns iterator to item after removed index.
	//WARNING: linear time.
	template <typename T,typename Alloc>
	typename darray<T>::iter remove(darray<T>* arr,uint32 index,Alloc* alloc);

	template <typename T>
	float32 load_factor(darray<T>* arr);

	template <typename T>
	typename darray<T>::iter begin_iter(darray<T>* arr);

	template <typename T>
	typename darray<T>::iter sub_iter(darray<T>* arr,
			uint32 begin,uint32 end);

	template <typename T>
	typename darray<T>::iter end_iter(darray<T>* arr);
}

#include "dggt_darray.inl"

#endif
