///exescope.h

#ifndef EXESCOPE_H
#define EXESCOPE_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif


#include "dispatcher.h"

NAMESP_BEGIN

template<class Thread, class Queue>
class ExeScopeBase
{
public:
	using scope_id = typename Thread::thread_id; 

public:
	ExeScopeBase(){
		_t = new Thread(&_q);
		assert(_t);
	}

	virtual ~ExeScopeBase(){
		try{
			_t->stop();
		}catch(...){
		}

		delete _t;
	}

	void stop(){
		try{
			_t->stop();
		}catch(...){
		}
	}

	template<typename Callee, typename... Params>
	void post(const Callee& e, Params... params){
		dispatch_asyn(&_q, e, params...);
	}

	template<typename Callee, typename... Params>
	void exec(const Callee& e, Params... params){
		if( isInMyScope() )
			e(params...);
		else
		{
            if(!_t->is_running()){
                throw std::runtime_error("scope is stopped");
            }
			dispatch_sync( &_q, e, params...);
		}
	}

	template<typename Callee, typename... Params>
	auto invoke(const Callee& e, Params... params){
		if( isInMyScope() )
			return e(params...);
		else
		{
            if(!_t->is_running()){
                throw std::runtime_error("scope is stopped");
            }
			return dispatch_invoke(&_q, e, params...);
		}
	}

	size_t queue_size(){ return _q.size(); }
	
	scope_id current_scope()const{ return Thread::get_curid(); }
	scope_id scope(){ return _t->getId(); }

    void name(const std::string& name){ _t->setName(name); }
    std::string name()const{ return _t->getName(); }

	bool isInMyScope(){
		return _t->getId() == Thread::get_curid();
	}
		
protected:
	Thread* _t;
	Queue _q;
};

NAMESP_END

#include "thread.h"
#include "executable_queue.h"
#include "executable_queue_polling.h"

NAMESP_BEGIN

using ExeScope = ExeScopeBase<Thread<ExecutableQueue>, ExecutableQueue>;

template<template<class> class ThreadP, class Poller>
class ExeScope_p_T :public  ExeScopeBase<ThreadP<ExecutableQueuePolling<Poller>>, ExecutableQueuePolling<Poller>>
{
public:
	ExeScope_p_T(Poller* p)
    {
        this->_q.setPoller(p); 
	}

	Poller* poller(){ return this->_q.getPoller(); }

};


template<class Poller>
using ExeScope_p = ExeScope_p_T<Thread, Poller>;

template<class Poller>
class CurExeScope :
   public ExeScope_p_T<CurThread, Poller>
{
public:
	CurExeScope (Poller* p)
       :ExeScope_p_T<CurThread, Poller>(p)
    {}
    void run(){
        this->_t->exec();
    }
};
NAMESP_END

#endif /*EXESCOPE_H*/
