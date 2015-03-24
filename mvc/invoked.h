//invoked.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify: 2014-12-19
//record 
//****************************

//****************************
//modify: 2014-12-26
//rename to invoked
//****************************

#ifndef INVOKED_H
#define INVOKED_H

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef _STRING_
#include <string>
#endif

#define INVOKED_BEGIN(n) \
	template<class Derived, class... Ts>\
	class Invoked<n, Derived, Ts...> \
	{\
	protected:\
		typedef Invoked<n, Derived, Ts...> base_t;\
	public:

#define INVOKED_PARAM(i) \
		static const int P##i = i;\
		typedef typename ArgAt<i, Ts...>::result 	p##i##_t;

	
#define INVOKED_END \
	};

NAMESP_BEGIN

//@N      : number of the params it want to accept,
//@Derived: the Derived class Derive from it,
//@Ts...  : params type of the Derived class want to accept, 
//          they ordered from left to right.
template<int N, class Derived, class ...Ts>
class Invoked;

//the want 1 params one.
template<class Derived, class... Ts>
class Invoked<1, Derived, Ts...> 
{
protected:
	typedef Invoked<1, Derived, Ts...> base_t;
	
public:
	enum{ P1, PC };	
	
	typedef typename ArgAt<0, Ts...>::result 	p1_t;
};

//the want 2 params one.
template<class Derived, class... Ts>
class Invoked<2, Derived, Ts...> 
{
protected:
	typedef Invoked<2, Derived, Ts...> base_t;

public:
	enum{ P1, P2, PC };	
	
	typedef typename ArgAt<0, Ts...>::result 	p1_t;
	typedef typename ArgAt<1, Ts...>::result 	p2_t;
};

//the want 3 params one.
template<class Derived, class... Ts>
class Invoked<3, Derived, Ts...> 
{
protected:
	typedef Invoked<3, Derived, Ts...> base_t;
	
public:
	enum{ P1, P2, P3, PC };	
	
	typedef typename ArgAt<0, Ts...>::result 	p1_t;
	typedef typename ArgAt<1, Ts...>::result 	p2_t;
	typedef typename ArgAt<2, Ts...>::result 	p3_t;
	
};


	

//the want 4 params one.
INVOKED_BEGIN(4)
	INVOKED_PARAM(0)
	INVOKED_PARAM(1)
	INVOKED_PARAM(2)
	INVOKED_PARAM(3)	
INVOKED_END



NAMESP_END

#endif /*INVOKED_H*/
