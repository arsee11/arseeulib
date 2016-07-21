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


#include <string>

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

using namespace std;



NAMESP_BEGIN

#ifdef __GNUC__
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





///////////////////////////////////////////////////////////////////////////////////
template<class RUNNABLE>
class Thread
{
public:
	typedef void (RUNNABLE::*RUNNER)();
	
	Thread(RUNNABLE *rnbl, RUNNER rn)
		:_fd(INVALID_FD)
	{
		if(rnbl==NULL)
			throw ThreadExp("NULL Pointer Argument!");

		_runnable = rnbl;
		_runner = rn;
	}

	~Thread()
	{
	}

	//创建并开始线程
	int start()
	{
		if(_fd == INVALID_FD)
		{
			return pthread_create(&_fd, 
					NULL, 
					run,
					this					
					);
		}
		else
			throw ThreadExp("Can't start more than once!'");
	}


	void join(int time_out=0)
	{
		int *rt;
		if( pthread_join(_fd, (void**)&rt) != 0 )
			throw ThreadExp("jion failed!");
	}
	
	

	
private:
	//
	void running()
	{
		if(_runnable != NULL &&  _runner != NULL)
			(_runnable->*_runner)();
		
	}

	//线程入口点
	static void* run(void *arg)
	{
		Thread<RUNNABLE> *me =
			static_cast<Thread<RUNNABLE>* >(arg);
		
		me->running();	
	}

private:
	RUNNABLE *_runnable; 	//作为线程的参数，由多个线程共享，所以定义时必须保证其是线程安全的。
	RUNNER _runner;		//客户自定义，作为线程的运行函数
	FD _fd;		
};

NAMESP_END/*namespace*/

#endif /*HLG_RUNNABLE_H*/
