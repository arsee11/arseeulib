//params_dispath.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-19
//record 
//****************************

#ifndef PARAMS_DISPATCH_H
#define PARAMS_DISPATCH_H

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef TYPE_CONVERT_H
#include "type_convert.h"
#endif

#include <string>
#include <map>

NAMESP_BEGIN

typedef std::map<std::string, std::string> param_pack_t;

template<class PARAM_PACK, class...PLACES>
struct ParamTransfor
{
	template<class T>
	T ToType(PARAM_PACK &ppack, std::string &param_name)
	{
		return std::move( TypeConvertor<T>::Convert( ppack[param_name] ) );
	}

	template<int N>
	typename ArgAt<N, PLACES...>::result::type Get()
	{
		typedef typename ArgAt<N, PLACES...>::result place_t;
		return ToType<typename place_t::type>(ppack, place_t::name);
	}
	
	PARAM_PACK ppack;
};

//@T: params type.
template<class T, int N>
struct ParamPlace
{
	//param name index base 0.
	enum{ Sn = N };

	//param name.
	static std::string name;

	//param type.
	typedef T type;
};

template<class T, int N>
std::string ParamPlace<T, N>::name = "";

///////////////////////////////////////////////////////
//Invoker
//@N: number of params the Invoked obj want to.
template<int N>
struct Invoker;

//the 0 params one.
template<>
struct Invoker<0>
{
	//@T: The Invoked obj.
	template<class Receiver, class SRC, class T>
	static int Invoke(Receiver *rev, SRC *src, T &t)
	{
		t->Execute(rev, src);
	}	
};

//the 1 params one.
template<>
struct Invoker<1>
{
	//@PARAM_PACK:params value pack,
	//@T		 :The Invoked obj.
	template<class Receiver, class SRC, class PARAM_PACK, class T>
	static int Invoke(Receiver* rev, SRC *src, const PARAM_PACK &pp, T *t)
	{
		typedef ParamPlace< typename T::p1_t,0> 	_1pp;	
			
		_1pp::name = T::_p1;
			
		ParamTransfor<param_pack_t, _1pp> pt = {pp};
		//template specify Get<0> is a template.
		t->Execute(rev, src, pt.template Get<0>() );
	}	
};

//the 2 params one.
template<>
struct Invoker<2>
{
	//@PARAM_PACK:params value pack,
	//@T		 :The Invoked obj.
	template<class Receiver, class SRC, class PARAM_PACK, class T>
	static int Invoke(Receiver* rev, SRC *src, const PARAM_PACK &pp, T *t)
	{
		typedef ParamPlace< typename T::p1_t,0> 	_1pp;	
		typedef ParamPlace< typename T::p2_t,1> 	_2pp;	
				
		_1pp::name = T::_p1;
		_2pp::name = T::_p2;
		//cout << T::_p1 << " " << T::_p2 << endl;
		//cout << _1pp::name << " " << _2pp::name << endl;
		ParamTransfor<param_pack_t, _1pp, _2pp> pt = {pp};
		return t->Execute(rev, src
			,pt.template Get<0>()
			,pt.template Get<1>()
		);
	}	
};

//the 3 params one.
template<>
struct Invoker<3>
{
	//@PARAM_PACK:params value pack,
	//@T		 :The Invoked obj.
	template<class Receiver, class SRC, class PARAM_PACK, class T>
	static int Invoke(Receiver* rev, SRC *src, const PARAM_PACK &pp, T *t)
	{
		typedef ParamPlace< typename T::p1_t, 0> 	_1pp;
		typedef ParamPlace< typename T::p2_t, 1> 	_2pp;
		typedef ParamPlace< typename T::p3_t, 2> 	_3pp;
		
		_1pp::name = T::_p1;
		_2pp::name = T::_p2;
		_3pp::name = T::_p3;
		
		ParamTransfor<param_pack_t, _1pp, _2pp, _3pp> pt = {pp};
		return t->Execute(rev, src
			, pt.template Get<0>()
			, pt.template Get<1>()
			, pt.template Get<2>() 
		);
	}	
};


NAMESP_END

#endif /*PARAMS_DISPATCH_H*/
