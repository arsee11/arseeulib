//file: request.h

#ifndef REQUEST_H
#define REQUEST_H

#include <memory>
#include <map>
#include <functional>

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

//#ifndef INVOKER_H
//#include "invoker.h"
//#endif

#ifndef REQUEST_CONTEXT_H
#include "request_context.h"
#endif

#ifndef RRESPONSE_H
#include "rresponse.h"
#endif

#ifndef OBJECT_LOGIC_H
#include "object_logic.h"
#endif


NAMESP_BEGIN

/*template<class Logic>
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
*/

template<class Pack>
class RRequestObject
{
private:
	typedef ObjectLogic<Pack> logic_t;
	typedef std::unique_ptr<logic_t > logic_ptr_t;

public:
	void AttachContext(RequestContext* val){ _context = val; }
	void AttachLogic(logic_t *logic)
	{
		_logic = logic_ptr_t(logic);
	}

	IRResponse<Pack>* Execute(const Pack &pack)
	{
		_logic->set_request_context(_context);
		typename Pack::object_list_t objs = pack.object_list();
		return _logic->Exec(objs); 
	}

private:
	logic_ptr_t 	_logic;
	RequestContext* _context = nullptr; 
};

NAMESP_END;

#endif /*REQUEST_H*/
