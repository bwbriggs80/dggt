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
	};

	template <typename T,typename Alloc>
	sllist<T> create_sllist(Alloc* alloc=0,uint32 size=0);

	template <typename T>
	uint32 get_count(sllst<T>* lst);

	//TODO: reserve should return number of allocated nodes.
	template <typename T,typename Alloc>
	bool32 reserve(sllst<T>* lst,uint32 count,Alloc* alloc);

	template <typename T,typename Alloc>
	bool32 push(sllst<T>* lst,Alloc* alloc);

	template <typename T,typename Alloc>
	bool32 push(sllst<T>* lst,T item,Alloc* alloc);

	//TODO: pop should return success bool32.
	template <typename T,typename Alloc>
	void pop(sllst<T>* lst,Alloc* alloc);

	template <typename T>
	typename sllst<T>::iter begin_iter(sllst<T>* lst);
}

#include "dggt_sllist.inl"

#endif
