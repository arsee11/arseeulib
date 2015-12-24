//rqh.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-15
//record 
//****************************

#ifndef RQH_H 
#define RQH_H 

#ifndef REQUEST_H
#include "request.h"
#endif

#ifndef CONTROL_H
#include "control.h"
#endif

#ifndef DISPATCHER_H
#include "dispatcher.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#include <memory>



NAMESP_BEGIN

template<class Pack, class Logic>
class RqhTrait
{
private:
	typedef RControl<Pack, Logic> ctrl_t;
	typedef typename ctrl_t::response_t rsp_t;
	
public:
	typedef Dispatcher<ctrl_t > dispth_t;	
};


NAMESP_END

#endif /*RQH_H*/
