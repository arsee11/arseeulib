///@file dispather.h

#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <mutex>
#include <condition_variable>
#include <functional>
#include <assert.h>

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif


NAMESP_BEGIN

template<class Queue, class Functor, class... Params>
int dispatch_asyn(Queue* q, const Functor& f, Params... params)
{
    assert(q != nullptr);
	{
		auto call = std::bind(f, params...);
		q->post(call);
	}
	
	return 0;
}


struct CallableWrapper
{
    template<class Functor, class... Params >
    CallableWrapper(const Functor& f, Params... params){
        _func = std::bind(f, params...);
    }

    void exec(){
        _func();

        {
            std::lock_guard<std::mutex> lck(_mtx);
            _has_called=true;
        }
        _condv.notify_all();
    }

    void wait(){
        std::unique_lock<std::mutex> lck(_mtx);
        while(!_has_called) //if exec function called before wait function called, ths will be ok;
            _condv.wait(lck);
    }

private:
    std::mutex _mtx;
    std::condition_variable _condv;
    bool _has_called=false;
    std::function<void()> _func=nullptr;
};

template<class Queue, class Functor, class... Params >
int dispatch_sync(Queue* q, const Functor& f, Params... params)
{
    assert(q != nullptr);
	{
        CallableWrapper c(f, params...);
        q->post( std::bind(&CallableWrapper::exec, &c) );
		c.wait();
	}
	
    return 0;
}

NAMESP_END

#endif /*DISPATCHER_H*/
