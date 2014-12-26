//logictmpl.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify: 2014-12-19
//record 
//****************************

#ifndef LOGIC_TMPL_H
#define LOGIC_TMPL_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef INVOKED_H
#include "invoked.h"
#endif

NAMESP_BEGIN

//BaseLogicTmpl
template<int n, class Derived, class Response, class... Ts>
class BaseLogicTmpl:
	public Invoked<n, Derived, Ts...> 
{
protected:
	Response *_rsp;

public:
	BaseLogicTmpl(Response *rsp)
		:_rsp(rsp)
	{}
	
	BaseLogicTmpl()=delete;	
};

NAMESP_END

#endif