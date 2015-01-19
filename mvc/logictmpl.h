//logictmpl.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify: 2014-12-19
//record 
//****************************
//****************************
//modify: 2015-01-05
//update 
//****************************

#ifndef LOGIC_TMPL_H
#define LOGIC_TMPL_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef INVOKED_H
#include "invoked.h"
#endif

#ifndef MVC_RRESPONSE_H
#include "mvcrresponse.h"
#endif

#ifndef JPACK_H
#include "jpack.h"
#endif

NAMESP_BEGIN

//BaseLogicTmpl
template<int n, class Derived, class Response, class PushResponse, class... Ts>
class BaseLogicTmpl:
	public Invoked<n, Derived, Ts...> 
{
public:
	typedef Response response_t;

protected:
	typedef BaseLogicTmpl<n, Derived, Response, PushResponse, Ts...> base_t;
	typedef PushResponse pusher_t;
	
public:
	BaseLogicTmpl()=default;
};

template<int n, class Derived, class... Ts>
using BaseLogicJpack = BaseLogicTmpl<n, Derived, RResponse<Jpack>, PushResponse<Jpack>, Ts...>;

NAMESP_END

#endif
