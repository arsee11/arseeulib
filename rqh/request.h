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

#ifndef INVOKER_H
#include "invoker.h"
#endif

#ifndef MVC_REQUEST_CONTEXT_H
#include "mvcrequest_context.h"
#endif

NAMESP_BEGIN

template<class Logic>
class RRequest
{
private:
	typedef Logic logic_t;
	typedef std::unique_ptr<logic_t > logic_ptr_t;

public:
	void AttachContext(RequestContext* val){ _context = val; }
	void AttachLogic(logic_t *logic)
	{
		_logic = logic_ptr_t(logic);
	}

	template<class OBJECT, class Argumet>
	auto Execute(OBJECT* obj, const Argumet &arg)
	->decltype(Invoker<logic_t::PC>::Invoke(obj, arg, (logic_t*)nullptr)) 
	{
		_logic->set_request_context(_context);
		return Invoker<logic_t::PC>::Invoke(obj, arg, _logic.get()); 
	}

private:
	logic_ptr_t 	_logic;
	RequestContext* _context = nullptr; 
};


//template<class SOURCE>
//class MultiRequest
//{
//public:
//	typedef SOURCE source_t;
//	typedef std::unique_ptr<LogicT<source_t> > logic_ptr_t;
//
//public:
//	void AttachSrc(source_t *src)
//	{
//		_src = src;
//	}
//	
//	void AttachLogic(int id, LogicT<source_t> *logic)
//	{
//		_logic[id] = logic_ptr_t(logic);
//	}
//	
//	void DettachLogic(int id)
//	{
//		_logic.erase(id );
//	}
//	
//	template<class PARAM_HANDER, class... Ts>
//	int Execute(PARAM_HANDER &ph, Ts... ts)
//	{
//		ph.Solve(_src, ts...);
//		int rt = 0;
//		for(auto &i : _logic)
//			rt = i.second->Execute( _src );
//			
//		return rt;
//	}
//	
//	template<class PARAM_HANDER, class... Ts>
//	int Execute(int id, PARAM_HANDER &ph, Ts... ts)
//	{
//		ph.Solve(_src, ts...);
//		auto i = _logic.find(id);
//		if( i != _logic.end() )
//			return i->second->Execute(_src);
//			
//		return 1;
//	}
//	
//	template<class PREDICATE, class PARAM_HANDER, class... Ts>
//	int Execute(PREDICATE pred, PARAM_HANDER* ph, Ts... ts)
//	{
//		std::unique_ptr<PARAM_HANDER>(ph)->Solve(_src, ts...);
//		int rt = 0;
//		for(auto &i : _logic)
//		{
//			auto bpred = std::bind(pred, i.first);
//			if( bpred() )
//				rt = i.second->Execute( _src );
//		}
//		
//		return rt;
//	}
//	
//	//bool Serialize();
//	//bool UnSerialize();
//	
//private:
//	std::map<int, logic_ptr_t> _logic;
//	source_t *_src;
//	
//};

NAMESP_END;

#endif /*MVC_REQUEST_H*/
