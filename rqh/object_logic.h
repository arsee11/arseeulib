//object_logic.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-26
//record 
//****************************

#ifndef OBJECT_LOGIC_H
#define OBJECT_LOGIC_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef REQUEST_CONTEXT_H
#include "request_context.h"
#endif

#ifndef RRESPONSE_H
#include "rresponse.h"
#endif

#ifndef IOBJECT_H
#include "iobject.h"
#endif

NAMESP_BEGIN

template<class Pack>
class ObjectLogic
{
public:
	typedef std::shared_ptr<IObject> object_ptr_t;
	typedef std::vector<object_ptr_t> object_list_t;
		
public:
	virtual ~ObjectLogic(){}
	virtual IRResponse<Pack>* Exec(object_list_t& objs)=0;
	void set_request_context(RequestContext* val){ _request_context = val; }
	RequestContext* get_request_context(){ return _request_context; }

private:
	RequestContext* _request_context =nullptr;
};

NAMESP_END

#endif /*OBJECT_LOGIC_H*/
