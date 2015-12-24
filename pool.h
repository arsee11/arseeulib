//thread_and_pool.h

#if defined(_MSC_VER_)
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(__GNUC__)
#include <pthread.h>
#define DWORD unsigned long
#endif

#include <list>
#include <algorithm>

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

NAMESP_BEGIN

class NullQueue{};

///////////////////////////////////////////////
///@Runable 可执行的对象
template<class Runable, class QueueItem=NullQueue>
class Thread
{
public:
	typedef QueueItem queue_item_t;
	typedef void* constructor_t;

public:
	template<class Context>
	Thread( Context *txt, bool suspend=false)
		:_stop(false)
	{
		_runable = new Runable(txt);
		Start(suspend);
		_runable->_handle = _handle;
	}

	Thread(bool suspend=false)
		:_stop(false)
	{
		_runable = new Runable();
		Start(suspend);
		_runable->_handle = _handle;
	}

	void Push(QueueItem *item){ _runable->Push(item);}
	QueueItem* Pop(){ return _runable->Pop(); }

	bool IsStop(){ return _stop; }
	void Stop()
	{	
		_stop = true;
		::WaitForSingleObject(_handle, 3000);
	}

	bool IsFree(){ return _runable->IsFree(); }

	void Release()
	{
		Stop();
		delete this;
	}

	void Suspend(){ ::SuspendThread(_handle); }
	void Resume(){ ::ResumeThread(_handle); }
	void Reset(){}

private:
	~Thread()
	{
		if(_runable != NULL)
			_runable->Release();
	}

	DWORD Start(bool suspend=false)
	{
		DWORD tid;
		_handle =  ::CreateThread(0, 0, Proc, (void *)this, suspend?CREATE_SUSPENDED:0, &tid);
		_tid = tid;
		return tid;
	}

	//thread 入口
	static DWORD __stdcall Proc(LPVOID lpParam)
	{
		Thread *t = static_cast<Thread*>(lpParam);
		printf("thread start, id:%d\n", t->_tid);
		while( !t->IsStop() )
		{
			t->Run();
		}

		printf("thread stop, id:%d\n", t->_tid);
		return 0;
	}

	void Run()
	{
		(*_runable)(_tid);
	}

	
	bool _stop;
	Runable *_runable;
	HANDLE _handle;
	DWORD _tid;
};


///////////////////////////////////////////////////////////////
//Pool
template<class Item, int max>
class Pool
{
public:
	typedef Item item_t;
	typedef typename item_t::constructor_t item_constructor_t;

private:
	typedef std::list<item_t*> item_list_t;
	typedef typename item_t::queue_item_t queue_item_t;
	typedef Pool<Item, max> my_t;
	typedef Thread<my_t > lookup_thread_t;
	friend class Thread<my_t >;
	

public:
	Pool()
	{
		::InitializeCriticalSection(&_guard);		
		_lookupt = new lookup_thread_t(this);
	}

	~Pool()
	{
		_lookupt->Release();

		for(item_list_t::iterator i =_busy_list.begin(); i!=_busy_list.end();)
		{
			item_list_t::value_type item  = (*i);
			i = _busy_list.erase(i);
			item->Release();			
		}

		for(item_list_t::iterator j =_free_list.begin(); j!=_free_list.end();)
		{
			item_list_t::value_type item  = (*j);
			j = _free_list.erase(j);
			item->Release();			
		}

		::DeleteCriticalSection(&_guard); 
	}

	Item* GetItem(item_constructor_t &cnstr)
	{
		::EnterCriticalSection(&_guard);
		Item *item = NULL;
		if(_free_list.size() >0)
			item = SchedFree();
		else
		{
			if( Size() < max )
			{
				item = new Item(cnstr);
			}
			else
				return NULL;
		}

		_busy_list.push_back(item);		
		::LeaveCriticalSection(&_guard);		
		
		item->Resume();
		return item;
	}

	template<class Context>
	Item* GetItem(queue_item_t *qitem, Context *txt)
	{
		::EnterCriticalSection(&_guard);
		Item *item = NULL;
		if(_free_list.size() >0)
			item = SchedFree();
		else
		{
			if( Size() < max )
			{
				item = new Item(txt, true);
			}
			else
				item = SchedBusy();
		}

		_busy_list.push_back(item);		
		::LeaveCriticalSection(&_guard);

		item->Push( qitem );
		
		item->Resume();
		return item;
	}

	size_t Size(){ return _busy_list.size() + _free_list.size(); }

	bool RealaseItem(Item *item)
	{
		::EnterCriticalSection(&_guard);
		item_list_t::iterator i = find(_busy_list.begin(), _busy_list.end(), item);
		if(i != _busy_list.end() )
			_busy_list.erase(i);

		item->Reset();
		_free_list.push_back(item);
		::LeaveCriticalSection(&_guard);

		return true;
	}

	void Release(){ }

private:
	Pool(my_t *myself)
	{
		_myself = myself;
	}

	void  Check()
	{
		::EnterCriticalSection(&_guard);
		item_list_t::iterator i =_busy_list.begin();
		for(; i!=_busy_list.end();)
		{
			item_list_t::value_type item  = (*i);
			if( item->IsFree() )
			{
				i = _busy_list.erase(i);
				_free_list.push_back(item);
				item->Reset();
			}
			else
				++i;
		}

		::LeaveCriticalSection(&_guard);
	}

	Pool(const Pool&);
	Pool& operator=(const Pool&);

	void operator()(DWORD tid)
	{
		_myself->Check();
		Sleep(200);
	}

	Item *SchedFree()
	{
		item_list_t::iterator i = _free_list.begin();
		Item *item =(*i);
		_free_list.erase(i);
		return item;
	}

	Item *SchedBusy()
	{
		item_list_t::iterator i = _busy_list.begin();
		Item *item =(*i);
		_busy_list.erase(i);
		return item;
	}


private:
	CRITICAL_SECTION _guard;
	item_list_t _busy_list;
	item_list_t _free_list;
	lookup_thread_t *_lookupt;
	HANDLE _handle;
	my_t *_myself;
};

NAMESP_END/*namespace*/
