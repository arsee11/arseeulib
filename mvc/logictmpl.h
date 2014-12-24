//logictmpl.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-19
//record 
//****************************

#ifndef LOGIC_TMPL_H
#define LOGIC_TMPL_H

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef PARAMS_DISPATCH_H
#include "params_dispatch.h"
#endif


NAMESP_BEGIN

//@N      : number of the params it want to accept,
//@Derived: the Derived class Derive from it,
//@Ts...  : params type of the Derived class want to accept, 
//          they ordered from left to right.
template<int N, class Derived, class ...Ts>
class BaseLogicTmpl;

//the want 1 params one.
template<class Derived, class Response, class... Ts>
class BaseLogicTmpl<1, Derived, Response, Ts...> 
{
protected:
	typedef BaseLogicTmpl<1, Derived, Response, Ts...> base_t;
	Response *_rsp;
	
public:
	BaseLogicTmpl(Response *rsp)
		:_rsp(rsp)
	{}
	
	BaseLogicTmpl()=delete;
	
	enum{ P1, PC };	
	
	typedef typename ArgAt<0, Ts...>::result 	p1_t;
		
	static const std::string _p1;
};

//the want 2 params one.
template<class Derived, class Response, class... Ts>
class BaseLogicTmpl<2, Derived, Response, Ts...> 
{
protected:
	typedef BaseLogicTmpl<2, Derived, Response, Ts...> base_t;
	Response *_rsp;

public:
	BaseLogicTmpl(Response *rsp)
		:_rsp(rsp)
	{}
	
	BaseLogicTmpl()=delete;
	
	enum{ P1, P2, PC };	
	
	typedef typename ArgAt<0, Ts...>::result 	p1_t;
	typedef typename ArgAt<1, Ts...>::result 	p2_t;

	static const std::string _p1;
	static const std::string _p2;
};

//the want 3 params one.
template<class Derived, class Response, class... Ts>
class BaseLogicTmpl<3, Derived, Response, Ts...> 
{
protected:
	typedef BaseLogicTmpl<3, Derived, Response, Ts...> base_t;
	Response *_rsp;

public:
	BaseLogicTmpl(Response *rsp)
		:_rsp(rsp)
	{}
	
	BaseLogicTmpl()=delete;
	
	enum{ P1, P2, P3, PC };	
	
	typedef typename ArgAt<0, Ts...>::result 	p1_t;
	typedef typename ArgAt<1, Ts...>::result 	p2_t;
	typedef typename ArgAt<2, Ts...>::result 	p3_t;
	
	static const std::string _p1;
	static const std::string _p2;
	static const std::string _p3;
	
	
	
};

NAMESP_END

#endif /*LOGIC_TMPL_H*/
