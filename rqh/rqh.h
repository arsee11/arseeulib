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
#include "mvcrequest.h"
#endif

#ifndef MVC_CONTROL_H
#include "mvccontrol.h"
#endif

#ifndef DISPATCHER_H
#include "dispatcher.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#include <memory>



NAMESP_BEGIN

template<class Pack, class ObjectsCollection, class Logic>
class MvcTrait
{
private:
	typedef RControl<Pack, Logic> ctrl_t;
	typedef typename ctrl_t::response_t rsp_t;
	
public:
	typedef Dispatcher<ctrl_t, ObjectsCollection> dispth_t;	
};

NAMESP_END

#endif /*MVC_H*/
