#ifndef _DGGT_SLLIST_H_
#define _DGGT_SLLIST_H_

#include "dggt_defs.h"

namespace dggt::coll
{
	template <typename T>
	struct slnode
	{
		T item;
		slnode<T>* next;
	};

	template <typename T>
	struct sllist
	{
		slnode<T>* head;
		uint32 count;

		struct iter
		{
			slnode<T>* current;
			union
			{
				slnode<T>* end;
				uint32 offset;
			};

			iter(slnode<T>* current,slnode<T>* endNode);
			iter(slnode<T>* current,uint32 offset);
			iter(slnode<T>* current);
			iter();

			bool32 is_end();
			bool32 next();

			T* get_item();
			const T* get_item() const;
			slnode<T>* get_node();
			const slnode<T>* get_node() const;

			T& operator*();
			const T& operator*() const;

			iter& operator++();
			const iter& operator++(int);
		};

		//WARNING: linear time
		typename darray<T>::iter operator[](uint32 index);
		//WARNING: linear time
		const typename darray<T>::iter operator[](uint32 index);
	};

	template <typename T,typename Alloc>
	sllist<T> create_sllist(Alloc* alloc=0,uint32 size=0);

	template <typename T>
	uint32 get_count(sllist<T>* lst);

	//TODO: return sub_iter
	template <typename T,typename Alloc>
	typename sllist<T>::iter reserve(sllist<T>* lst,uint32 count,Alloc* alloc);

	//TODO: pop should return iterator to new head.
	template <typename T,typename Alloc>
	typename sllist<T>::iter push(sllist<T>* lst,Alloc* alloc);

	//TODO: pop should return iterator to new head.
	template <typename T,typename Alloc>
	typename sllist<T>::iter push(sllist<T>* lst,T item,Alloc* alloc);

	//TODO: returns iterator to toRemove->next
	template <typename T,typename Alloc>
	typename sllist<T>::iter remove(sllist<T>* lst,slnode<T>* prev,
			slnode<T>* toRemove,Alloc* alloc);

	//TODO: returns iterator to newly inserted, uninitialized node.
	template <typename T,typename Alloc>
	typename sllist<T>::iter insert(sllist<T>* lst,slnode<T>* prev,
			Alloc* alloc);

	//TODO: returns iterator to newly inserted, initialized node.
	template <typename T,typename Alloc>
	typename sllist<T>::iter insert(sllist<T>* lst,slnode<T>* prev,
			T item,Alloc* alloc);

	//TODO: returns iterator to newly inserted, uninitialized node.
	//WARNING: linear time.
	template <typename T,typename Alloc>
	typename sllist<T>::iter insert(sllist<T>* lst,uint32 prevIndex,
			Alloc* alloc);

	//TODO: returns iterator to newly inserted, initialized node.
	//WARNING: linear time.
	template <typename T,typename Alloc>
	typename sllist<T>::iter insert(sllist<T>* lst,uint32 prevIndex,
			T item,Alloc* alloc);

	//TODO: pop should return iterator to new head.
	template <typename T,typename Alloc>
	typename sllist<T>::iter pop(sllist<T>* lst,Alloc* alloc);

	template <typename T>
	typename sllist<T>::iter begin_iter(sllist<T>* lst);

	template <typename T>
	typename sllist<T>::iter sub_iter(sllist<T>* lst,
			slnode<T>* begin,slnode<T>* end);

	template <typename T>
	typename sllist<T>::iter sub_iter(sllist<T>* lst,
			slnode<T>* begin,uint32 offset);
}

#include "dggt_sllist.inl"

#endif
