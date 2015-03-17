//mvcresponse.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-15
//record 
//****************************

#ifndef MVC_RRESPONSE_H
#define MVC_RRESPONSE_H

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef STRINGEX_H
#include "../stringex.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#include <string>
#include <memory>
#include <list>

#ifndef MVC_VIEW_H
#include "mvcview.h"
#endif

NAMESP_BEGIN

/////////////////////////////////////////////////
//RResponse
//Remote Response
template<class Pack>
class RResponse
{	
public:
	typedef typename _name::view_t view_t;
	typedef Pack pack_t;
	typedef typename pack_t::params_pack_t 	params_pack_t;
	typedef typename pack_t::pack_ptr_t  	pack_ptr_t;
	typedef typename pack_t::pack_list_t 	pack_list_t;
	
public:
	RResponse(const std::string &name, view_t &view)
		:_name(name)
		,_view(view)
	{
	}
	
	RResponse(std::string &&name, view_t &view)
		:RResponse(name,view)
	{}
	
	RResponse(const std::string &name)
		:_name(name)
	{}
	
	RResponse(std::string &&name)
		:RResponse(name)
	{}
	
	void view(const view_t &view){ _view = view; }
		
	template<class T>
	void ParamAdd(std::string &&key, const T& value)
	{ 
		_params[key] = StringBuilder(value); 
	}
	
	pack_t* Reply()
	{
		if(_params.size() > 0)
			return new pack_t(_name::_name, _view, "response", _params);
			
		return nullptr;
	}
				
protected:
	view_t _view;
	params_pack_t _params;
	std::string _name;
	//pack_list_t _pcks;
	//std::string _src;
	//std::string _target;
};


/////////////////////////////////////////////////
//RResponse
//Remote Response
template<class Pack>
class PushResponse
{
public:
	typedef Pack pack_t;
	typedef typename pack_t::params_pack_t 	params_pack_t;
	typedef typename pack_t::pack_ptr_t  	pack_ptr_t;
	typedef typename pack_t::pack_list_t 	pack_list_t;
	
public:
	PushResponse(const std::string &name, const view_t &view)
		:_name(name)
		,_view(view)
	{
	}
	
	PushResponse(std::string &&name, view_t &&view)
		:PushResponse(name,view)
	{}
	
	template<class T>
	void ParamAdd(std::string &&key, const T& value)
	{ 
		_params[key] = StringBuilder(value); 
	}
	
		
	template<class Sender>	
	void Push(Sender& ss)
	{
		pack_t pck(_name::_name, _view, "push", _params);
		typename pack_t::serial_t s;
		size_t len=0;
		const char *buf = s(pck, &len);
		ss(buf, len);
		delete[] buf;
	}
	
				
protected:
	view_t _view;
	params_pack_t _params;
	std::string _name;
	//pack_list_t _pcks;
	//std::string _src;
	//std::string _target;
};

NAMESP_END;

#endif /*MVC_RRESPONSE_H*/
