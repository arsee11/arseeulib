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

//#include <Windows.h>
#include <memory>

using namespace std;

template<class BUFFER>
class BufferState
{
protected:
	typedef typename BUFFER::guard_t guard_t;
	typedef typename BUFFER::value_t value_t;
	typedef typename BUFFER::value_ptr_t value_ptr_t;
	typedef typename BUFFER::item_ptr_t item_ptr_t;
	
public:
	BufferState(BUFFER *h)
		:_holder(h)
	{}

	virtual void Full() = 0;
	virtual void Free() = 0;
	virtual void Empty() = 0;
	virtual value_t& Get(value_t& value) = 0;
	virtual void Put(value_t& value) = 0;

protected:
	BUFFER *_holder;
};


template<class BUFFER>
class BufferEmptyState:
	public BufferState<BUFFER>
{
public:
	BufferEmptyState(BUFFER* h)
		:BufferState(h)
	{}

	void Full() override
	{
		_holder->_state = _holder->_st_full;
		//ResetEvent(_holder->_free_cond);
	}
	
	void Free() override
	{
		_holder->_state = _holder->_st_free;
		_holder->_free_cond.NotifyOne();
	}
	
	void Empty() override
	{
	}
	
	value_t& Get(value_t &value) override
	{
		_holder->_free_cond.Wait(_holder->_block_lock.get());
		return _holder->Pop(value);
	}
	
	void Put(value_t& value) override
	{
		_holder->Put_atomic(value);	
	
	}
};

template<class BUFFER>
class BufferFullState :
	public BufferState<BUFFER>
{
public:
	BufferFullState(BUFFER* h)
		:BufferState(h)
	{}

	void Full() override
	{
	}
	
	void Free() override
	{
		_holder->_state = _holder->_st_free;
		_holder->_free_cond.NotifyOne();
	}
	
	void Empty() override
	{
		_holder->_state = _holder->_st_empty;
		//ResetEvent(_holder->_free_cond);
	}
	
	value_t& Get(value_t& value) override
	{
		return _holder->Get_atomic(value);
	}
	
	void Put(value_t& value) override
	{
		_holder->_free_cond.Wait(_holder->_block_lock.get());
		_holder->Push(value);	
	}
};

template<class BUFFER>
class BufferFreeState :
	public BufferState<BUFFER>
{
public:
	BufferFreeState(BUFFER* h)
		:BufferState(h)
	{}

	void Full() override
	{
		_holder->_state = _holder->_st_full;
		//ResetEvent(_holder->_free_cond);
	}
	
	void Free() override
	{
		//_holder->_free_cond.NotifyOne();
	}
	
	void Empty() override
	{
		_holder->_state = _holder->_st_empty;
		//ResetEvent(_holder->_free_cond);
	}
	
	value_t& Get(value_t& value) override
	{
		return _holder->Get_atomic(value);
	}
	
	void Put(value_t& value) override
	{
		_holder->Put_atomic(value);
	}
};


///////////////////////////////////////////////////
//producer-consumer buffer 
template<class VALUE>
class BufferItem
{
	typedef BufferItem<VALUE> my_t;

	friend class BufferEmptyState<my_t>;
	friend class BufferFreeState<my_t>;
	friend class BufferFullState<my_t>;

public:
	typedef VALUE value_t;
	typedef value_t *value_ptr_t;
	typedef const value_t *const_value_ptr_t;

	
	BufferItem()
	{
		_st_empty = shared_ptr<BufferEmptyState<my_t> >(new BufferEmptyState<my_t>(this));
		//_st_block = shared_ptr<BufferBlockState<my_t> >(new BufferBlockState<my_t>(this));
		_st_full = shared_ptr<BufferFullState<my_t> >(new BufferFullState<my_t>(this));
		_state = _st_empty;
		_full_sem = CreateEvent(NULL, TRUE, FALSE, NULL);
		_empty_sem = CreateEvent(NULL, TRUE, TRUE, NULL);
	}
	
	~BufferItem()
	{}
	
	void Put(value_ptr_t val)
	{
		_state->Put(val);
	}

	value_ptr_t Get(value_ptr_t value)
	{
		return _state->get(value);
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

	void Put_atomic(value_ptr_t val, size_t len)
	{
		_size = len;
		_value = val;
	}

	value_ptr_t Get_atomic(value_ptr_t value)
	{
		memcpy(value, _value, _size);
		return value;
	}
		
public:
	value_ptr_t _value	= nullptr;
	//typename guard_t::lockable_t _block_lock; 
	shared_ptr<BufferEmptyState<my_t> > _st_empty;
	//shared_ptr<BufferBlockState<my_t> > _st_block;
	shared_ptr<BufferFullState<my_t> > 	_st_full;
	shared_ptr<BufferState<my_t> > 	_state;
	HANDLE _full_sem;
	HANDLE _empty_sem;
};



///////////////////////////////////////////////
//CharPtrWrapper.
//Wrapper the char* type. 
struct CharPtrWrapper
{
	void Copy(const char* src, size_t size)
	{
		if (this->buf != nullptr && this->size != size)
		{
			delete[] this->buf;
			this->buf = nullptr;
		}

		this->buf = new char[size];
		memcpy(buf, src, size);
		this->size = size;
	}

	CharPtrWrapper(){}

	CharPtrWrapper(const CharPtrWrapper& rhs) = delete;
	

	CharPtrWrapper(CharPtrWrapper&& rhs)
	{
		//cout << "move contr" << endl;
		if (this != &rhs)
		{
			if (this->buf != nullptr)
			{
				delete[] buf;
				this->buf = nullptr;
			}

			this->buf = rhs.buf;
			this->size = rhs.size;
			rhs.buf = nullptr;
			rhs.size = 0;
		}
	}

	CharPtrWrapper& operator=(const CharPtrWrapper& rhs) = delete;

	CharPtrWrapper& operator=(CharPtrWrapper&& rhs)
	{
		//cout << "move assign" << endl;
		if (this != &rhs)
		{
			if (this->buf != nullptr)
			{
				delete[] buf;
				this->buf = nullptr;
			}

			this->buf = rhs.buf;
			this->size = rhs.size;
			rhs.buf = nullptr;
			rhs.size = 0;
		}
		return (*this);
	}

	~CharPtrWrapper()
	{
		if (this->buf != nullptr)
		{
			delete[] buf;
			buf = nullptr;
		}
	}

	char *buf	= nullptr;
	size_t size	= 0;
};



#endif/*BUFFERED_H*/