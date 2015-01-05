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

#ifndef MVCRESPONSE_H
#include "mvcresponse.h"
#endif

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

#ifndef MVC_VIEW_H
#include "mvcview.h"
#endif

#include <list>

NAMESP_BEGIN

/////////////////////////////////////////////////
//RResponse
//Remote Response
template<class Pack>
class RResponse
	:public Response<std::string, Pack>
{
public:
	typedef typename pack_t::params_pack_t 	params_pack_t;
	typedef typename pack_t::pack_ptr_t  	pack_ptr_t;
	typedef typename pack_t::pack_list_t 	pack_list_t;
	
public:
	RResponse(const std::string &name, view_t &view)
		:Response(name)
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
		_params[pname] = StringBuilder(pvalue); 
	}
	
	pack_t* Reply()
	{
		if(_params.size() > 0)
			return new pack_t(_name, _view, "response", _params);
			
		return nullptr;
	}
				
protected:
	view_t _view;
	params_pack_t _params;
	//pack_list_t _pcks;
	//std::string _src;
	//std::string _target;
};


/////////////////////////////////////////////////
//RResponse
//Remote Response
template<class Pack>
class PushResponse
	:public Response<std::string, Pack>
{
public:
	typedef typename pack_t::params_pack_t 	params_pack_t;
	typedef typename pack_t::pack_ptr_t  	pack_ptr_t;
	typedef typename pack_t::pack_list_t 	pack_list_t;
	
public:
	RResponse(const std::string &name, const view_t &view)
		:Response(name)
		,_view(view)
	{
	}
	
	RResponse(std::string &&name, view_t &&view)
		:RResponse(name,view)
	{}
	
	template<class T>
	void ParamAdd(std::string &&key, const T& value)
	{ 
		_params[pname] = StringBuilder(pvalue); 
	}
	
		
	template<class Sender>	
	void Push(Sender& ss)
	{
		pack_t pck(_name, _view, "push", _params)
		ss(pck);
	}
	
				
protected:
	view_t _view;
	params_pack_t _params;
	//pack_list_t _pcks;
	//std::string _src;
	//std::string _target;
};

NAMESP_END;

#endif /*MVC_RRESPONSE_H*/
