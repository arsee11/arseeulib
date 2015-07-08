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

//typedef std::map<std::string, std::string> param_pack_t;

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

NAMESP_END

#endif /*PARAMS_DISPATCH_H*/
