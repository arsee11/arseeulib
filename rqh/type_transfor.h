//type_transfor.h
#include "globaldef.h"
#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef TYPE_TRANSFOR_H
#define TYPE_TRANSFOR_H

NAMESP_BEGIN

//transfor type to other type.
//@N number of params the Other tmplate has,
//@Other other type template class,
//@Us... the params will Other use, every U must define a trans_t type.
template<int N,template<class...>class Other, class...Us>
struct mt;

//1 param.	
template<template<class...>class Other, class...Us>
struct mt<1, Other, Us...>
{
	typedef typename Other<
		typename ArgAt<0,Us...>::result::trans_t
	> a_t;	
};

//2 params.
template<template<class...>class Other, class...Us>
struct mt<2, Other, Us...>
{
	typedef typename Other<
		 typename ArgAt<0,Us...>::result::trans_t
		,typename ArgAt<1,Us...>::result::trans_t
	> a_t;	
};


//3 params.
template<template<class...>class Other, class...Us>
struct mt<3, Other, Us...>
{
	typedef typename ArgAt<0,Us...>::result::trans_t t1;
	typedef typename ArgAt<1,Us...>::result::trans_t t2;	
	typedef typename ArgAt<2,Us...>::result::trans_t t3;

	
};


//Transfor to Other<Us...> type.
template<template<class...>class Other, class...Us>
struct TransType
{	
	
	typedef typename mt<ArgCounter<Us...>::value, Other, Us...>::a_t result;
	//typedef T type	
	//typedef Other<type> result;
	//typedef ArgIteration<Args...>::type types;
	
};


NAMESP_END

#endif/*TYPE_TRANSFOR_H*/
