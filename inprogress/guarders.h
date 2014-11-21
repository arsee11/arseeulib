//guarders.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-11-12
//create 
//****************************


#ifndef GUARDERS_H
#define GUARDERS_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#if defined(_MSC_VER)
#include <windows.h>
#endif/*_MSC_VER*/

#if defined(__GUNC__)
#include <pthread.h>
#endif

class lockexp:
	public std::exception
{
public:
	lockexp(const char *str)
		:exception(str)
	{
	}
	
	void trace(std::iostream &stream);
};

template<class LOCKABLE>
class Guard
{
public:
	typedef LOCKABLE lockable_t;
	typedef lockable_t* lockable_ptr_t;

public:
	Guard(lockable_t *lck)
		:_lck(lck)
	{
		_lck->Lock();
	}

	template<class Interval>
	Guard(lockable_t *lck, Interval &timeout)
	{
		_lck->Lock(timeout);
	}


	~Guard()
	{
		_lck->UnLock();
	}

private:
	lockable_ptr_t _lck;
};

template<class T
	, class InitHandler
	, class LockHandler
	, class UnLockHanlder
	, class UnInitHandler>
class Lockable
{
public:
	Lockable()throw(lockexp)
	{
		_initer(&lock);
	}
	
	~Lockable()
	{
		_deleter(&lock);
	}
	
	template<class Interval>
	void Lock(Interval &timeout)throw(lockexp)
	{
		_locker(&lock, timeout);
	}
	
	template<class Interval>
	void Lock(Interval timeout)throw(lockexp)
	{
		_locker(&lock, timeout);
	}

	void Lock()throw(lockexp)
	{
		_locker(&lock);
	}
	
	void UnLock()throw(lockexp)
	{
		_unlocker(&lock);
	}	
	
	T* get()const{ return &lock; }
	
private:
	T lock;
	InitHandler		_initer;
	LockHandler		_deleter;
	UnLockHanlder	_locker;
	UnInitHandler	_unlocker;	
};


////////////////////////////////////////////////////////////
#if defing(__GUNC__)
class Condvar
{
public:
	Condvar()
	{
		if( pthread_cond_init(&_cond, NULL) > 0)
		{
			perror("pthread_cond_init");
			throw lockexp("pthread_cond_init");
		}
	}
	
	~Condvar()
	{
		if( pthread_cond_destroy(&_cond, NULL) > 0)
		{
			perror("pthread_cond_destroy");
		}
	}
	
	template<class Lockable>
	void Wait(Lockable *lck)
	{
		pthread_cond_wait(&_cond, lock);
	}
	
	void NotityOne()
	{
		pthread_cond_signal(&_cond);
	}
	
	void NotityAll()
	{
		pthread_cond_broadcast(&_cond);
	}
	
private:
	pthread_cond_t _cond;
};

struct MutexIniter
{
	void operator()(pthread_mutex_t* mtx)
	{
		pthread_mutex_int(mtx, NULL);
	}
};

struct MutexDeleter
{
	void operator()(pthread_mutex_t* mtx)
	{
		pthread_mutex_destroy(mtx);
	}
};

struct MutexLocker
{
	void operator()(pthread_mutex_t* mtx)
	{
		::pthread_mutex_lock(mtx);
	}
};

struct MutexUnlocker
{
	void operator()(pthread_mutex_t* mtx)
	{
		::pthread_mutex_unlock(mtx);
	}
};

typedef Lockable<pthread_mutex_t>
	, MutexIniter
	, MutexDeleter
	, MutexLocker
	, MutexUnlocker
>
mutex_t;
		
typedef Guard<mutex_t> mutex_guard_t;

#endif /*__GUNC__*/

//////////////////////////////////////////////////
#if defined(_MSC_VER)
struct CSIniter
{
	void operator()(CRITICAL_SECTION* cs)
	{
		::InitializeCriticalSection(cs);
	}
};

struct CSDeleter
{
	void operator()(CRITICAL_SECTION* cs)
	{
		::DeleteCriticalSection(cs);
	}
};

struct CSLocker
{
	void operator()(CRITICAL_SECTION* cs)
	{
		::EnterCriticalSection(cs);
	}
};

struct CSUnlocker
{
	void operator()(CRITICAL_SECTION* cs)
	{
		::LeaveCriticalSection(cs);
	}
};

typedef Lockable<CRITICAL_SECTION
	, CSIniter
	, CSDeleter
	, CSLocker
	, CSUnlocker
>
critical_section_t;
		
typedef Guard<critical_section_t> critical_section_guard_t;
			
#endif /*_MSC_VER*/


#endif /*GUARDERS_H*/