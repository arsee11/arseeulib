//invoker.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-26
//record 
//****************************

#ifndef INVOKER_H
#define INVOKER_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef PARAMS_DISPATCH_H
#include "params_dispatch.h"
#endif

NAMESP_BEGIN

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
	static auto Invoke(SRC *src, T &t)
	->decltype(t->Execute(src))
	{
		return t->Execute(src);
	}	
};

//the 1 params one.
template<>
struct Invoker<1>
{
	//@PARAM_PACK:params value pack,
	//            [{name:value},{name:value}].
	//@T		 :The Invoked obj.
	template<class SRC, class PARAM_PACK, class T>
	static auto Invoke(SRC *src, const PARAM_PACK &pp, T *t)
	->decltype(t->Execute(src, typename T::p1_t() ))
	{
		typedef ParamPlace< typename T::p1_t,0> 	_1pp;	
			
		_1pp::name = T::p1();	
		ParamTransfor<PARAM_PACK, _1pp> pt = { pp };
		//template specify Get<0> is a template.
		return t->Execute(src, pt.template Get<0>() );
	}	
	
	//@PARAM_PACK:params value pack
	//            [{name:value},{name:value}].
	//@T		 :The Invoked obj.
	template<class PARAM_PACK, class T>
	static int Invoke(const PARAM_PACK &pp, T *t)
	{
		typedef ParamPlace< typename T::p1_t,0> 	_1pp;	
			
		_1pp::name = T::p1();
			
		ParamTransfor<PARAM_PACK, _1pp> pt = { pp };
		//template specify Get<0> is a template.
		return t->Execute(pt.template Get<0>() );
	}	
};

//the 2 params one.
template<>
struct Invoker<2>
{
	//@PARAM_PACK:params value pack,
	//@T		 :The Invoked obj.
	template<class SRC, class PARAM_PACK, class T>
	static auto Invoke(SRC *src, const PARAM_PACK &pp, T *t)
	->decltype( t->Execute(src, typename T::p1_t(), typename T::p2_t()) )
	{
		typedef ParamPlace< typename T::p1_t,0> 	_1pp;	
		typedef ParamPlace< typename T::p2_t,1> 	_2pp;	
				
		_1pp::name = T::p1();
		_2pp::name = T::p2();
		//cout << T::_p1 << " " << T::_p2 << endl;
		//cout << _1pp::name << " " << _2pp::name << endl;
		ParamTransfor<PARAM_PACK, _1pp, _2pp> pt = { pp };
		return t->Execute(src
			,pt.template Get<0>()
			,pt.template Get<1>()
		);
	}	

	//@PARAM_PACK:params value pack,
	//@T		 :The Invoked obj.
	template<class PARAM_PACK, class T>
	static int Invoke(const PARAM_PACK &pp, T *t)
	{
		typedef ParamPlace< typename T::p1_t, 0> 	_1pp;
		typedef ParamPlace< typename T::p2_t, 1>	_2pp;

		_1pp::name = T::p1();
		_2pp::name = T::p2();

		ParamTransfor<PARAM_PACK, _1pp, _2pp> pt = { pp };
		//template specify Get<0> is a template.
		return t->Execute(pt.template Get<0>()
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
	static auto Invoke(SRC *src, const PARAM_PACK &pp, T *t)
	->decltype( t->Execute(src, typename T::p1_t(), typename T::p2_t(), typename T::p3_t()) )
	{
		typedef ParamPlace< typename T::p1_t, 0> 	_1pp;
		typedef ParamPlace< typename T::p2_t, 1> 	_2pp;
		typedef ParamPlace< typename T::p3_t, 2> 	_3pp;
		
		_1pp::name = T::p1();
		_2pp::name = T::p2();
		_3pp::name = T::p3();
		
		ParamTransfor<PARAM_PACK, _1pp, _2pp, _3pp> pt = { pp };
		//cout<<typeid(pt.template Get<0>()).name()<< pt.template Get<0>()<<typeid(pt.template Get<1>()).name()<< pt.template Get<1>()<<typeid(pt.template Get<2>()).name()<< pt.template Get<2>()<<endl; 
		return t->Execute(src
			, pt.template Get<0>()
			, pt.template Get<1>()
			, pt.template Get<2>() 
		);
	}	

	// @PARAM_PACK:params value pack,
	//@T		 :The Invoked obj.
	template<class PARAM_PACK, class T>
	static int Invoke(const PARAM_PACK &pp, T *t)
	{
		typedef ParamPlace< typename T::p1_t, 0> 	_1pp;
		typedef ParamPlace< typename T::p2_t, 1> 	_2pp;
		typedef ParamPlace< typename T::p3_t, 2> 	_3pp;

		_1pp::name = T::p1();
		_2pp::name = T::p2();
		_3pp::name = T::p3();

		ParamTransfor<PARAM_PACK, _1pp, _2pp, _3pp> pt = { pp };
		//template specify Get<0> is a template.
		return t->Execute(pt.template Get<0>()
			, pt.template Get<1>()
			, pt.template Get<2>()
			);
	}
};

//the 4 params one.
template<>
struct Invoker<4>
{
	//@PARAM_PACK:params value pack,
	//@T		 :The Invoked obj.
	template<class SRC, class PARAM_PACK, class T>
	static auto Invoke(SRC *src, const PARAM_PACK &pp, T *t)
	->decltype( t->Execute(src, typename T::p1_t(), typename T::p2_t(), typename T::p3_t()) )
	{
		typedef ParamPlace< typename T::p1_t, 0> 	_1pp;
		typedef ParamPlace< typename T::p2_t, 1> 	_2pp;
		typedef ParamPlace< typename T::p3_t, 2> 	_3pp;
		typedef ParamPlace< typename T::p3_t, 3> 	_4pp;
		
		_1pp::name = T::p1();
		_2pp::name = T::p2();
		_3pp::name = T::p3();
		_4pp::name = T::p4();
		
		ParamTransfor<PARAM_PACK, _1pp, _2pp, _3pp, _4pp> pt = { pp };
		return t->Execute(src
			, pt.template Get<0>()
			, pt.template Get<1>()
			, pt.template Get<2>()
			, pt.template Get<3>() 
		);
	}	

	// @PARAM_PACK:params value pack,
	//@T		 :The Invoked obj.
	template<class PARAM_PACK, class T>
	static int Invoke(const PARAM_PACK &pp, T *t)
	{
		typedef ParamPlace< typename T::p1_t, 0> 	_1pp;
		typedef ParamPlace< typename T::p2_t, 1> 	_2pp;
		typedef ParamPlace< typename T::p3_t, 2> 	_3pp;
		typedef ParamPlace< typename T::p3_t, 3> 	_4pp;
		
		_1pp::name = T::p1();
		_2pp::name = T::p2();
		_3pp::name = T::p3();
		_4pp::name = T::p4();
		
		ParamTransfor<PARAM_PACK, _1pp, _2pp, _3pp, _4pp> pt = {pp};
		//template specify Get<0> is a template.
		return t->Execute(pt.template Get<0>()
			, pt.template Get<1>()
			, pt.template Get<2>()
			, pt.template Get<3>()
			);
	}
};


NAMESP_END

#endif /*INVOKER_H*/
