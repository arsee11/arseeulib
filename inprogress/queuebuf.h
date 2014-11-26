//buffered.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify	
//data	: 2014-11-12
//log	: create 
//****************************


#ifndef QUEUEBUF_H
#define QUEUEBUF_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <queue>
#include <memory>
#include <string.h>

#ifndef BUFFERED_H
#include "buffered.h"
#endif

#ifndef GUARDERS_H
#include "guarders.h"
#endif
using namespace std;


///////////////////////////////////////////////////////////////////////////
//FIFO buffer, adapt producer-consumer mode.
//It can work on 1..n producers and 1..n consumers.
//@VALUE type of buffer contained.
//GUARD, locker type.
template<class VALUE, class GUARD>
class QueueBuffer
{
	typedef QueueBuffer<VALUE, GUARD> my_t;
public:
	typedef VALUE value_t;
	typedef value_t *value_ptr_t;
	typedef const value_t * const_value_ptr_t;

	typedef VALUE item_t;
	typedef item_t* item_ptr_t;
	typedef std::queue<item_t> container_t;

	typedef GUARD guard_t;

	friend class BufferEmptyState<my_t>;
	friend class BufferFreeState<my_t>;
	friend class BufferFullState<my_t>;

public:
	explicit QueueBuffer(size_t max_size)
		:_max_size(max_size)
	{
		_st_empty = shared_ptr<BufferEmptyState<my_t> >(new BufferEmptyState<my_t>(this));
		_st_free = shared_ptr<BufferFreeState<my_t> >(new BufferFreeState<my_t>(this));
		_st_full = shared_ptr<BufferFullState<my_t> >(new BufferFullState<my_t>(this));
		_state = _st_empty;
	}

	//return size of buffer(used).
	size_t size()
	{
		guard_t g(&_block_lock);
		return _queue.size();
	}

	//return size of buffer left(not use).
	size_t free_size()
	{
		guard_t g(&_block_lock);
		return _max_size - size();
	}
	
	void WakeUp()
	{
		//SetEvent(_full_sem);
		//SetEvent(_empty_sem);
	}

	value_t& Pop(value_t& value)
	{
		guard_t g(&_block_lock);
		return _state->Get(value);
	}

	void Push(value_t& value)
	{
		guard_t gf(&_block_lock);
		_state->Put(value);
	}

private:
	void Full()
	{
		_state->Full();
	}

	void Empty()
	{
		_state->Empty();
	}

	void Free()
	{
		_state->Free();
	}

	value_t& Get_atomic(value_t &value)
	{
		//cout<<"begin get..."<<endl;
		//guard_t gf(&_block_lock);
		item_t &item = _queue.front();
		value = std::move(item);
		_queue.pop();

		if (size() == 0)
			Empty();
		else
			Free();
				
		return value;
	}

	void Put_atomic(value_t &value)
	{
		//guard_t gf(&_block_lock);
		_queue.push(std::forward<value_t>(value));

		if (free_size() == 0)
			Full();
		else
			Free();
	}

private:
	shared_ptr<BufferEmptyState<my_t> > _st_empty;
	shared_ptr<BufferFreeState<my_t> > 	_st_free;
	shared_ptr<BufferFullState<my_t> > 	_st_full;

	shared_ptr<BufferState<my_t> > 	_state;

	Condvar _free_cond;
	typename guard_t::lockable_t _block_lock;

	container_t _queue;
	size_t _max_size;
};

	
///////////////////////////////////////////////////
//specialize for the char* type.
template<class Guard>
class QueueBuffer<char*, Guard> :
	public QueueBuffer<CharPtrWrapper, Guard>
{
	typedef QueueBuffer<CharPtrWrapper, Guard> base_t;
	
public:
	explicit QueueBuffer(size_t max_size) :base_t(max_size){}

	//Pop() func get a copy of the buffer tail.
	//@size specify bytes of @value.
	//@return size of @value.
	//allocate space for the value , so users must release 
	//the space manul.
	//if buffer is empty, it will block the caller, untill 
	//not empty.
	size_t Pop(char** value)
	{
		CharPtrWrapper item;
		base_t::Pop(item);
		memcpy(value, item.buf, item.size);
		return item.size;
	}

	//Push() func make a copy of the @value, and push the
	//copy into the buffer tail.
	//@size specify bytes of @value.
	//if buffer is full, it will block the caller, untill 
	//not full.
	void Push(char* value, size_t size)
	{
		CharPtrWrapper item;
		item.Copy(value, size);
		base_t::Push(item);
	}
};

#endif/*QUEUEBUF_H*/