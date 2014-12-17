//params_dispath.h

#ifndef PARAMS_DISPATCH_H
#define PARAMS_DISPATCH_H

#ifndef GLOBALDEF_H
#include "globaldef.h"
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

//@N      : number of the params it want to accept,
//@Derived: the Derived class Derive from it,
//@Ts...  : params type of the Derived class want to accept, 
//          they ordered from left to right.
template<int N, class Derived, class ...Ts>
class BaseLogicTmpl;

//the want 1 params one.
template<class Derived, class... Ts>
class BaseLogicTmpl<1, Derived, Ts...> 
{
protected:
	typedef BaseLogicTmpl<1, Derived, Ts...> base_t;
	IResponse *_rsp;
	
public:
	BaseLogicTmpl(IResponse *rsp)
		:_rsp(rsp)
	{}
	
	BaseLogicTmpl()=delete;
	
	enum{ P1, PC };	
	
	typedef typename ArgAt<0, Ts...>::result 	p1_t;
		
	static const std::string _p1;
};

//the want 2 params one.
template<class Derived, class... Ts>
class BaseLogicTmpl<2, Derived, Ts...> 
{
protected:
	typedef BaseLogicTmpl<2, Derived, Ts...> base_t;
	IResponse *_rsp;

public:
	BaseLogicTmpl(IResponse *rsp)
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
template<class Derived, class... Ts>
class BaseLogicTmpl<3, Derived, Ts...> 
{
protected:
	typedef BaseLogicTmpl<3, Derived, Ts...> base_t;
	IResponse *_rsp;

public:
	BaseLogicTmpl(IResponse *rsp)
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
	template<class SRC, class T>
	static int Invoke(SRC *src, T &t)
	{
		t->Execute(src);
	}	
};

//the 1 params one.
template<>
struct Invoker<1>
{
	//@PARAM_PACK:params value pack,
	//@T		 :The Invoked obj.
	template<class SRC, class PARAM_PACK, class T>
	static int Invoke(SRC *src, const PARAM_PACK &pp, T *t)
	{
		typedef ParamPlace< typename T::p1_t,0> 	_1pp;	
			
		_1pp::name = T::_p1;
			
		ParamTransfor<param_pack_t, _1pp> pt = {pp};
		//template specify Get<0> is a template.
		t->Execute(src, pt.template Get<0>() );
	}	
};

//the 2 params one.
template<>
struct Invoker<2>
{
	//@PARAM_PACK:params value pack,
	//@T		 :The Invoked obj.
	template<class SRC, class PARAM_PACK, class T>
	static int Invoke(SRC *src, const PARAM_PACK &pp, T *t)
	{
		typedef ParamPlace< typename T::p1_t,0> 	_1pp;	
		typedef ParamPlace< typename T::p2_t,1> 	_2pp;	
				
		_1pp::name = T::_p1;
		_2pp::name = T::_p2;
		//cout << T::_p1 << " " << T::_p2 << endl;
		//cout << _1pp::name << " " << _2pp::name << endl;
		ParamTransfor<param_pack_t, _1pp, _2pp> pt = {pp};
		return t->Execute(src
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
	template<class SRC, class PARAM_PACK, class T>
	static int Invoke(SRC *src, const PARAM_PACK &pp, T *t)
	{
		typedef ParamPlace< typename T::p1_t, 0> 	_1pp;
		typedef ParamPlace< typename T::p2_t, 1> 	_2pp;
		typedef ParamPlace< typename T::p3_t, 2> 	_3pp;
		
		_1pp::name = T::_p1;
		_2pp::name = T::_p2;
		_3pp::name = T::_p3;
		
		ParamTransfor<param_pack_t, _1pp, _2pp, _3pp> pt = {pp};
		return t->Execute(src
			, pt.template Get<0>()
			, pt.template Get<1>()
			, pt.template Get<2>() 
		);
	}	
};


NAMESP_END

#endif /*PARAMS_DISPATCH_H*/
