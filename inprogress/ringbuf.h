//buffered.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify	
//data	: 2014-11-12
//log	: create 
//****************************


#ifndef BUFFERED_H
#define BUFFERED_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <Windows.h>
#include <queue>
#include <memory>

using namespace std;

///////////////////////////////////////////////////
//FIFO buffer, adapt producer-consumer mode.
//It can work on 1..n producers and 1..n consumers.
template<class VALUE, class GUARD>
class CircularBuffer
{	
	typedef CircularBuffer<VALUE, GUARD> my_t;
public:
	typedef VALUE value_t;
	//typedef BufferItem<value_t,GUARD> item_t;
	typedef VALUE item_t;	
	typedef item_t* item_ptr_t;
	typedef item_ptr_t container_t;
	typedef value_t *value_ptr_t;
	typedef const value_t * const_value_ptr_t;
	typedef GUARD guard_t;

	friend class BufferEmptyState<my_t>;
	friend class BufferFreeState<my_t>;
	friend class BufferFullState<my_t>;
	
public:
	explicit CircularBuffer(size_t max_size)
		:_max_size(max_size)
	{
		_queue = new item_t[max_size];
		//_rd_ptr = _wr_ptr = _queue;
		
		_st_empty = shared_ptr<BufferEmptyState<my_t> >(new BufferEmptyState<my_t>(this));
		_st_free = shared_ptr<BufferFreeState<my_t> >(new BufferFreeState<my_t>(this));
		_st_full = shared_ptr<BufferFullState<my_t> >(new BufferFullState<my_t>(this));
		_state = _st_empty;
		_full_sem = CreateEvent(NULL, TRUE, FALSE, NULL);
		_empty_sem = CreateEvent(NULL, TRUE, TRUE, NULL);
	}
	
	size_t size()
	{
		return _size;
	}
	
	size_t free_size()
	{
		return _max_size - size();
	}
	
	value_t& Pop(value_t& value)
	{
		return _state->Get(value);
	}
	
	void Push(value_t& value)
	{
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
		guard_t gf(&_block_lock);
		item_t &item = _queue[_rd_cursor];
		value = std::move(item);
		_size > 0? --_size : _size;
		//cout << "Get, _size:" << _size << endl;
		if (size() == 0)
			Empty();
		else
			Free();

		_rd_cursor = (++_rd_cursor % _max_size);
		//cout<<"get completed"<<endl;
		return value;
	}

	void Put_atomic(value_t &value)
	{
		guard_t gf(&_block_lock);
		item_t &item = _queue[_wr_cursor];
		item = std::move(value);
		_size < _max_size ? ++_size : _size;
		//cout << "Put, _size:" << _size << endl;

		if (free_size() == 0)
			Full();
		else
			Free();

		_wr_cursor = (++_wr_cursor %_max_size);		
	}
	
private:
	shared_ptr<BufferEmptyState<my_t> > _st_empty;
	shared_ptr<BufferFreeState<my_t> > 	_st_free;
	shared_ptr<BufferFullState<my_t> > 	_st_full;

	shared_ptr<BufferState<my_t> > 	_state;

	HANDLE _full_sem;
	HANDLE _empty_sem;
	typename guard_t::lockable_t _block_lock;
	
	container_t _queue	= nullptr;
	size_t _max_size;
	size_t _size		= 0;
	size_t _wr_cursor	= 0;
	size_t _rd_cursor	= 0;
	item_ptr_t _rd_ptr;
	item_ptr_t _wr_ptr;
};


///////////////////////////////////////////////////
//specialize for the char* type.
template<class Guard>
class CircularBuffer<char*, Guard> :
	public CircularBuffer<CharPtrWrapper, Guard>
{
		typedef CircularBuffer<CharPtrWrapper, Guard> base_t;
public:
	explicit CircularBuffer(size_t max_size) :base_t(max_size){}

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
		char *p = new char[itme.size];
		memcpy(value, item.buf, item.size);
		return value;
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
		//item.buf = value;
		//item.size = size;
		base_t::Push(item);
	}
};


	

#endif/*BUFFERED_H*/