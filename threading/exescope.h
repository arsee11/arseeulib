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
	ExeScopeBase(){
		_t = new Thread(&_q);
		assert(_t);
	}

	~ExeScopeBase(){
		_t->stop();
		delete _t;
	}

	template<typename Executee, typename... Params>
	void post(const Executee& e, Params... params){
		dispatch_asyn( &_q, e, params...);
	}

	template<typename Executee, typename... Params>
	void exec(const Executee& e, Params... params){
		if( isInMyScope() )
			e(params...);
		else
		{
			dispatch_sync( &_q, e, params...);
		}
	}

	size_t queue_size(){ return _q.size(); }
	
private:
	bool isInMyScope(){
		return _t.getId() == Thread::get_curid();
	}
		
private:
	Thread _t;
	Queue _q;
};

NAMESP_END

#include "thread.h"
#include "executable_queue.h"

NAMESP_BEGIN

using ExeScope = ExeScopeBase<Thread<ExecutableQueue>, ExecutableQueue>;

NAMESP_END

#endif /*EXESCOPE_H*/
