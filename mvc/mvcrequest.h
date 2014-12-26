//file: mvcrequest.h

#ifndef MVC_REQUEST_H
#define MVC_REQUEST_H

#include <memory>
#include <map>
#include <functional>

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef PARAMS_DISPATCH_H
#include "params_dispatch.h"
#endif

NAMESP_BEGIN

//ToDo::限制只能从new 创建
template<class OBJ>
class LogicT
{
public:
	typedef OBJ obj_t;
	
	virtual int Execute(obj_t *obj) = 0;

	virtual ~LogicT(){ /*cout << "~LogicT()" << endl;*/ }
};

template<class SOURCE, class LOGIC>
class Request
{
private:
	typedef SOURCE source_t;
	typedef LOGIC logic_t;
	typedef std::unique_ptr<logic_t > logic_ptr_t;

public:

	void AttachSrc(source_t *src)
	{
		_src = src;
	}

	void AttachLogic(logic_t *logic)
	{
		_logic = logic_ptr_t(logic);
	}
	
	template<class... Ts>
	int Execute(Ts... ts)
	{
		return _logic->Execute(_src, ts...);
	}

	template<class... Ts>
	int Execute(Ts*... ts)
	{
		return _logic->Execute(_src, ts...);
	}
	
	//bool Serialize();
	//bool UnSerialize();
	
private:
	logic_ptr_t _logic;
	source_t *_src;
	
};

template<class LOGIC, class Receiver>
class RRequest
{
private:
	//typedef SOURCE source_t;
	typedef LOGIC logic_t;
	typedef std::unique_ptr<logic_t > logic_ptr_t;

public:

//	void AttachSrc(source_t *src)
//	{
//		_src = src;
//	}

	void AttachReceiver(Receiver* rev){ _receiver = rev; }
	void AttachLogic(logic_t *logic)
	{
		_logic = logic_ptr_t(logic);
	}


	template<class OBJECT, class PARAMS_PACK>
	int Execute(OBJECT* obj, PARAMS_PACK &params)
	{
		return Invoker<logic_t::PC>::Invoke(_receiver, obj, params, _logic.get()); 
		//return _logic->Execute(_src, params);
	}

private:
	logic_ptr_t _logic;
	//source_t *_src;
	Receiver* _receiver = nullptr; 
};


template<class SOURCE>
class MultiRequest
{
public:
	typedef SOURCE source_t;
	typedef std::unique_ptr<LogicT<source_t> > logic_ptr_t;

public:
	void AttachSrc(source_t *src)
	{
		_src = src;
	}
	
	void AttachLogic(int id, LogicT<source_t> *logic)
	{
		_logic[id] = logic_ptr_t(logic);
	}
	
	void DettachLogic(int id)
	{
		_logic.erase(id );
	}
	
	template<class PARAM_HANDER, class... Ts>
	int Execute(PARAM_HANDER &ph, Ts... ts)
	{
		ph.Solve(_src, ts...);
		int rt = 0;
		for(auto &i : _logic)
			rt = i.second->Execute( _src );
			
		return rt;
	}
	
	template<class PARAM_HANDER, class... Ts>
	int Execute(int id, PARAM_HANDER &ph, Ts... ts)
	{
		ph.Solve(_src, ts...);
		auto i = _logic.find(id);
		if( i != _logic.end() )
			return i->second->Execute(_src);
			
		return 1;
	}
	
	template<class PREDICATE, class PARAM_HANDER, class... Ts>
	int Execute(PREDICATE pred, PARAM_HANDER* ph, Ts... ts)
	{
		std::unique_ptr<PARAM_HANDER>(ph)->Solve(_src, ts...);
		int rt = 0;
		for(auto &i : _logic)
		{
			auto bpred = std::bind(pred, i.first);
			if( bpred() )
				rt = i.second->Execute( _src );
		}
		
		return rt;
	}
	
	//bool Serialize();
	//bool UnSerialize();
	
private:
	std::map<int, logic_ptr_t> _logic;
	source_t *_src;
	
};

NAMESP_END;

#endif /*MVC_REQUEST_H*/
