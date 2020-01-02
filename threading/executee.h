//executee.h


#ifndef EXECUTEE_H
#define EXECUTEE_H

#include "exescope.h"
#include <mutex>

NAMESP_BEGIN

template<class Impl, class MultiThreadLocker>
class Executee 
{
	using lock_guard_t= std::lock_guard<MultiThreadLocker>;

public:
	///Executee object must live longer than the ExeScope object;
	Executee(ExeScope* exc)
		:_exc(exc)
	{}

	Executee()
	{}

	virtual ~Executee(){ cout<<__FUNCTION__<<endl;}

	void setScope(ExeScope* exc){ 
		lock_guard_t g(_locker);
		_exc = exc; 
	}

	template<typename MemFunc, typename... Args>
	void post(MemFunc mf, Args... args){
		lock_guard_t g(_locker);
		if(_exc != nullptr){
			_exc->post( mf, static_cast<Impl*>(this), args...);
		}
		else{
			///warning no executor
		}
	}


	ExeScope::scope_id current_scope(){ 
		lock_guard_t g(_locker);
		if(_exc != nullptr)
			return _exc->current_scope(); 

		return ExeScope::scope_id();
	}

	ExeScope::scope_id scope(){ 
		lock_guard_t g(_locker);
		if(_exc != nullptr)
			return _exc->scope(); 

		return ExeScope::scope_id();
	}

private:
	ExeScope* _exc=nullptr;
	MultiThreadLocker _locker;
};

struct NoneLocker
{
	void lock(){}
	void unlock(){}
};

template<class Impl>
	using Executee_st = Executee<Impl, NoneLocker>;

template<class Impl>
	using Executee_mt = Executee<Impl, std::mutex>;

NAMESP_END

#endif/*EXECUTEE_H*/
