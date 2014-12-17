//mvccontrol.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-15
//record 
//****************************

#ifndef MVC_H
#define MVC_H

#ifndef MVC_REQUEST_H
#include "mvc/mvcrequest.h"
#endif

#ifndef MVC_CONTROL_H
#include "mvc/mvccontrol.h"
#endif

#ifndef DISPATCHER_H
#include "mvc/dispatcher.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif


NAMESP_BEGIN

template<class Logic, class Object>
class MvcTrait
{
private:
	typedef RControl<Object, Logic> ctrl_t;
	typedef member_login_ctrl_t::response_t rsp_t;
	
public:
	typedef Dispatcher<member_login_ctrl_t> dispth_t;	
};

NAMESP_END

#endif /*MVC_H*/