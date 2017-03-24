#ifndef DEQUE_H
#define DEQUE_H

#include <string.h>
#include <stdlib.h>

#define STRINGIFY(x) #x
#define INIT_SIZE 10

#define Deque_DEFINE(t)										\
	struct Deque_##t##_Iterator {								\
		/* points to current position in deque iterator is pointing to */		\
		t* cur;										\
		/* points to the head position in deque */					\
		t* q_first;									\
		/* ponits to tail position in deque */						\
		t* q_last;									\
		/* increments the iterator & points to next location in queue */		\
		void (*inc)(Deque_##t##_Iterator* dIp);						\
		/* decrements the iterator & points to previous location in queue */		\
		void (*dec)(Deque_##t##_Iterator* dIp);						\
		/* Derefernces the *cur to get value in deque at current location pointed by cur pointer */\
		t& (*deref)(Deque_##t##_Iterator* dIp);						\
	};											\
	struct Deque_##t {									\
		t* data;		/* holds the base address of queue of data */		\
		int head;									\
		int tail;									\
		char type_name[sizeof(STRINGIFY(Deque_##t))];					\
		int count;		/* holds the number of elements present in queue*/	\
		int arr_size;									\
		Deque_##t##_Iterator it_begin;							\
		Deque_##t##_Iterator it_end;							\
		bool (*empty)(Deque_##t*);							\
		size_t (*size)(Deque_##t*);							\
		void (*push_back)(Deque_##t*, t data);						\
		void (*push_front)(Deque_##t*, t data);						\
		void (*pop_back)(Deque_##t*);							\
		void (*pop_front)(Deque_##t*);							\
		t& (*front)(Deque_##t*);							\
		t& (*back)(Deque_##t*);								\
		t& (*at)(Deque_##t*, int i);							\
		Deque_##t##_Iterator (*end)(Deque_##t*);					\
		Deque_##t##_Iterator (*begin)(Deque_##t*);					\
		void (*dtor)(Deque_##t*);							\
		void (*clear)(Deque_##t*);							\
	};											\
	/* allocates the memory as & when required 
	   mostly it is required when the number of elements try to exceed the array_size
	*/											\
	void alloc(Deque_##t* dp, t** new_data)							\
	{											\
		int size = dp->arr_size * 10 + 1;						\
		*new_data = (t*)malloc(size * sizeof(t));					\
	}											\
	/* Copies all elements in previous deque to newly created deque by alloc function */	\
	void copy_el(Deque_##t* dp, t** new_data, bool pushfront = false)			\
	{											\
		int index = 0;									\
		if(!pushfront)									\
		{										\
			/* copy all data from head to last */					\
			for(int i = dp->head; i < dp->arr_size; ++i)				\
			{									\
				(*new_data)[index++] = dp->data[i];				\
			}									\
			/* if tail is less than head copy all the data 
			from 0th position in deque till	tail*/									\
			if(dp->tail <= dp->head)						\
			{									\
				for(int j = 0; j < dp->tail; ++j)				\
				{								\
					(*new_data)[index++] = dp->data[j];			\
				}								\
			}									\
		}										\
		else if(dp->head < dp->tail)							\
		{										\
			for(int i = dp->tail; i > dp->head; ++i)				\
			{									\
				(*new_data)[i] = dp->data[i - 1];				\
			}									\
		}										\
		dp->arr_size = dp->arr_size * 10;						\
		free(dp->data);									\
		dp->head = 0;									\
		dp->tail = index;								\
	}											\
	/* checks if deque has any elements.
	   returns true if 0 elements, false otherwise */					\
	bool empty(Deque_##t* dp)								\
	{											\
		return (dp->count == 0)?true : false;						\
	}											\
	/* returns the count of elements present in deque */					\
	size_t size(Deque_##t* dp)								\
	{											\
		return dp->count;								\
	}											\
	/* points to the head of the deque & returns the iterator */				\
	Deque_##t##_Iterator begin(Deque_##t* dp)						\
	{											\
		dp->it_begin.cur = &(dp->front(dp));						\
		dp->it_begin.q_first = &(dp->data[0]);						\
		dp->it_begin.q_last = &(dp->data[dp->arr_size - 1]);				\
		return dp->it_begin;								\
	}											\
	/* points to the tail of the deque & returns the iterator */				\
	Deque_##t##_Iterator end(Deque_##t* dp)							\
	{											\
		dp->it_end.cur = &(dp->data[dp->tail]);						\
		dp->it_end.q_first = &(dp->data[0]);						\
		dp->it_end.q_last = &(dp->data[dp->arr_size - 1]);				\
		return dp->it_end;								\
	}											\
	/* pushes the new element at the tail of the deque */					\
	void push_back(Deque_##t *dp, t el)							\
	{											\
		/* If data array is full */							\
		if(dp->count == dp->arr_size)							\
		{										\
			t *new_data;								\
			alloc(dp, &new_data);							\
			copy_el(dp, &new_data);							\
			dp->data = new_data;							\
		}										\
		if(dp->head > 0 && dp->tail > dp->arr_size)					\
		{										\
			dp->tail = 0;								\
		}										\
		dp->data[dp->tail] = el;							\
		dp->tail++;									\
		dp->count++;									\
	}											\
	/* pushes the element at the head of the deque */					\
	void push_front(Deque_##t *dp, t el)							\
	{											\
		if(dp->count < dp->arr_size)							\
		{										\
			if(dp->tail > dp->head)							\
			{									\
				if(dp->tail < dp->arr_size)					\
				{								\
					dp->head = dp->arr_size - 1;				\
					dp->data[dp->head] = el;				\
				}								\
			}									\
			else if(dp->tail < dp->head && dp->head - dp->tail > 1)			\
			{									\
				dp->data[--(dp->head)] = el;					\
			}									\
		}										\
		else										\
		{										\
			t *new_data;								\
			alloc(dp, &new_data);							\
			copy_el(dp, &new_data, true);						\
			dp->data = new_data;							\
			dp->data[dp->head] = el;						\
		}										\
		dp->count++;									\
	}											\
	/* removes the element from the tail of the deque */					\
	void pop_back(Deque_##t *dp)								\
	{											\
		dp->count--;									\
		dp->tail--;									\
		if(dp->tail < 0)								\
		{										\
			dp->tail = dp->arr_size - 1;						\
		}										\
	}											\
	/* removes the element from the head of the deque */					\
	void pop_front(Deque_##t *dp)								\
	{											\
		if(dp->head == dp->arr_size - 1)						\
		{										\
			dp->head = 0;								\
		}										\
		else										\
		{										\
			dp->head++;								\
		}										\
		dp->count--;									\
	}											\
	/* returns the element at the head of the deque */					\
	t& front(Deque_##t *dp)									\
	{											\
		return dp->data[dp->head];							\
	}											\
	/* returns the element at the tail of the deque */					\
	t& back(Deque_##t *dp)									\
	{											\
		return dp->data[dp->tail - 1];							\
	}											\
	/* returns the element at ith position in deque */					\
	t& at(Deque_##t *dp, int i) 								\
	{				                               				\
		if((dp->head + i) >= dp->arr_size)						\
		{										\
			return dp->data[(dp->head + i) - dp->arr_size];		          	\
		}										\
		return dp->data[dp->head + i];							\
	}											\
	void dtor(Deque_##t *dp)								\
	{											\
		free(dp->data);									\
	}											\
	/* clears all the elements in deque */							\
	void clear(Deque_##t *dp)								\
	{											\
		memset(dp->data, 0, dp->arr_size * sizeof(t));					\
		dp->count = 0;									\
	}											\
	/* checks if two iterators are equal by comparing their current ponter			\ 
	   returns true if yes, false otherwise */						\
	bool Deque_##t##_Iterator_equal(const Deque_##t##_Iterator it1, Deque_##t##_Iterator it2)	\
	{											\
		if(it1.cur == it2.cur)								\
		{										\
			return true;								\
		}										\
		return false;									\
	}											\
	/* increments the iterator by one position in deque */					\
	void inc(Deque_##t##_Iterator* dIp)							\
	{											\
		if(dIp->cur != dIp->q_last)							\
		{										\
			(dIp->cur)++;								\
		}										\
		else										\
		{										\
			dIp->cur = dIp->q_first;						\
		}										\
	}											\
	/* decrements the iterator by one position in deque */					\
	void dec(Deque_##t##_Iterator* dIp)							\
	{											\
		if(dIp->cur != dIp->q_first)							\
		{										\
			(dIp->cur)--;								\
		}										\
		else										\
		{										\
			dIp->cur = dIp->q_last;							\
		}										\
	}											\
	/* returns the object pointed by iterator at curent position */				\
	t& deref(Deque_##t##_Iterator* dIp)							\
	{											\
		return *(dIp->cur);								\
	}											\
												\
	bool (*fpCompare_##t)(const t& t1, const t& t2);					\
												\
	void Deque_##t##_ctor(Deque_##t* dp, bool (*fp)(const t& t1, const t& t2))		\
	{											\
		dp->data = (t*)malloc(INIT_SIZE * sizeof(t));					\
		dp->head = 0;									\
		dp->tail = 0;									\
		dp->arr_size = INIT_SIZE;							\
		dp->count = 0;									\
		strcpy(dp->type_name, STRINGIFY(Deque_##t)); 					\
		dp->empty = empty;								\
		dp->size = size;								\
		dp->push_back = push_back;							\
		dp->push_front = push_front;							\
		dp->pop_back = pop_back;							\
		dp->pop_front = pop_front;							\
		dp->front = front;								\
		dp->back = back;								\
		dp->at = at;									\
		dp->end = end;									\
		dp->begin = begin;								\
		dp->dtor = dtor;								\
		dp->clear = clear;								\
		dp->it_begin.inc = inc;								\
		dp->it_begin.dec = dec;								\
		dp->it_begin.deref = deref;							\
		dp->it_end.inc = inc;								\
		dp->it_end.dec = dec;								\
		dp->it_end.deref = deref;							\
		fpCompare_##t = fp;								\
	}											\
	/* compares two deques element by element						\ 
	   returns true if yes, false otherwise */						\
	bool Deque_##t##_equal(Deque_##t& dp1, Deque_##t& dp2)					\
	{											\
		if(dp1.count == dp2.count)							\
		{										\
			for(int i = 0; i < dp1.count && i < dp2.count; ++i)			\
			{									\
				if(!fpCompare_##t(dp1.at(&dp1, i), dp2.at(&dp2, i)) 		\
					&& !fpCompare_##t(dp2.at(&dp2, i), dp1.at(&dp1, i)))	\
					;							\
				else								\
					return false;						\
			}									\
			return true;								\
		}										\
		return false;									\
	}
	
#endif

