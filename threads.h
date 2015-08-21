/*
***************************
* 文 件 名：hlg_runnable.h
* 摘    要：线程
* 作    者：黄立观
* 版    本：0.1
* 替换版本：0.0
* 更新时间：2012-03-17
**************************
*/

#ifndef HLG_RUNNABLE_H
#define HLG_RUNNABLE_H

#ifndef SYN_OBJ_H
#include "hlg_thr_synobj.h"
#endif

#include <string>

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

using namespace std;
using namespace arsee::thr;



NAMESP_BEGIN

#if define(__GNU_C__)
typedef pthread_t FD; //
#endif

#define INVALID_FD -1

	///////////////////////////////////////////////////////////////////////
//Thead Exception
class ThreadExp
{
public:
	ThreadExp(string msg)
		:Msg(msg)		
	{
	}

	string Msg;
};



	///////////////////////////////////////////////////////////////////////
class Thread
{
public:
	template<class... Args>
	Thread(RUNNABLE &rnbl, Args... &args)
	{

		Start();
	}

	virtual ~RunnableThread();

	//创建并开始线程
	int Start()
	{
		return pthread_create(&_fd, 
				NULL, 
				Work,
				this					
				);
	}

	void Terminate();
	void Wait(int time_out);

private:
	//
	void WorkWork()
	{
		(_runnable->*_runner)();
	}

	//线程入口点
	static void Work(void *arg)
	{
		RunnableThread<SYNSWITCH, SYNOBJ, RUNNABLE> *me =
			static_cast<RunnableThread<SYNSWITCH, SYNOBJ, RUNNABLE>* >(arg);
		
		me->WorkWork();	
	}

private:
	RUNNABLE *_runnable; 	//
	RUNNER _runner;		//
	SYNOBJ _synobj;		//
	FD _fd;		
};



///////////////////////////////////////////////////////////////////////////////////
template<class RUNNABLE>
class RunnableThread<false, NullSynObj, RUNNABLE>
{
public:
	typedef void (RUNNABLE::*RUNNER)();
	
	RunnableThread(RUNNABLE *rnbl, RUNNER rn)
		:_fd(INVALID_FD),
		 _state(true)
	{
		if(rnbl==NULL)
			throw ThreadExp("NULL Pointer Argument!");

		_runnable = rnbl;
		_runner = rn;
	}

	~RunnableThread()
	{
		Terminate();
	}

	//创建并开始线程
	int Start()
	{
		if(_fd == INVALID_FD)
		{
			return pthread_create(&_fd, 
					NULL, 
					Work,
					this					
					);
		}
		else
			throw ThreadExp("Can't start more than once!'");
	}

	void Terminate()
	{
		if(_fd != INVALID_FD)
		{				
			_state = false;
			Wait(0);
			_runnable->Cancel();
			_fd = INVALID_FD;
		}			
	}

	void Wait(int time_out)
	{
		int *rt;
		if( pthread_join(_fd, (void**)&rt) != 0 )
			throw ThreadExp("jion failed!");
	}
	
	

	
private:
	//
	void WorkWork()
	{
		if(_runnable == NULL || _runner == NULL)
			throw ThreadExp("NULL pointer detected!");
		while(_state)
		{
			(_runnable->*_runner)();
		}
		
		int rtval = 1;
		pthread_exit(&rtval);
	}

	//线程入口点
	static void* Work(void *arg)
	{
		RunnableThread<false, NullSynObj, RUNNABLE> *me =
			static_cast<RunnableThread<false, NullSynObj, RUNNABLE>* >(arg);
		
		me->WorkWork();	
	}

private:
	RUNNABLE *_runnable; 	//作为线程的参数，由多个线程共享，所以定义时必须保证其是线程安全的。
	RUNNER _runner;		//客户自定义，作为线程的运行函数
	NullSynObj _synobj;	//同步对象
	FD _fd;		
	bool _state;
};

NAMESP_END/*namespace*/

#endif /*HLG_RUNNABLE_H*/
