//rresponse.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-15
//record 
//****************************

#ifndef RRESPONSE_H
#define RRESPONSE_H

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

NAMESP_BEGIN

template<class Pack>
class IRResponse
{
public:
	typedef Pack pack_t;
	typedef typename pack_t::pack_ptr_t  	pack_ptr_t;
	typedef typename pack_t::pack_list_t 	pack_list_t;
	
	IRResponse(const std::string &name, std::string &view)
		:_name(name)
		,_view(view)
	{
	}
	
	IRResponse(std::string &&name, std::string &view)
		:IRResponse(name,view)
	{}
	
	IRResponse(const std::string &name)
		:_name(name)
	{}
	
	IRResponse(std::string &&name)
		:IRResponse(name)
	{}
	
	void view	(const std::string&		val){ _view   = val; }
	void action	(const std::string& val){ _action = val; }
	
	virtual pack_t* Reply()=0;
	virtual void Success()=0;
	virtual void Fail()=0;
	
protected:
	std::string _view;
	std::string _name;
	std::string _action;
};



/////////////////////////////////////////////////
//RResponse
//Remote Response
template<class Pack>
class RResponse:public IRResponse<Pack>
{
public:
	typedef typename IRResponse<Pack>::pack_t pack_t;
	typedef typename pack_t::object_ptr_t 	object_ptr_t;
	typedef typename pack_t::object_list_t 	object_list_t;
	typedef IRResponse<Pack> base_t;
public:
	RResponse(const std::string &name, std::string &view)
		:base_t(name, view)
	{
	}

	RResponse(std::string &&name, std::string &view)
		:RResponse(name, view)
	{}

	RResponse(const std::string &name)
		:base_t(name)
	{}

	RResponse(std::string &&name)
		:RResponse(name)
	{}

	
	void append_object(const object_ptr_t& obj)
	{
		_objs.push_back(obj);
	}

	virtual void Success(){}
	virtual void Fail(){};

	pack_t* Reply()
	{
		pack_t* pck = new pack_t(base_t::_action, "response");
		pck->status(true);
		if (_objs.size() > 0)
		{
			for (auto &i : _objs)
				pck->add_object(i);

		}
		return pck;
	}

protected:
	object_list_t _objs;
	
};


/////////////////////////////////////////////////
//PushResponse
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
	PushResponse(const std::string &name, std::string& view)
		:_name(name)
		,_view(view)
	{
	}

	PushResponse(const std::string& view)
		:_name("pusher")
		, _view(view)
	{
	}
	
	PushResponse(std::string&& view)
		:PushResponse(view)
	{
	}

	PushResponse(const char* view)
		:PushResponse(std::string(view))
	{
	}
	
	PushResponse(const char* name, std::string& view)
		:PushResponse(std::string(name), view)
	{
	}

	PushResponse(std::string &&name, std::string &&view)
		:PushResponse(name,view)
	{}
	
	template<class T>
	void add_param(std::string &&key, const T& value)
	{ 
		_pack_item[key] = StringBuilder(value); 
	}


	template<class T>
	void add_param(const char* key, const T value)
	{
		_pack_item[key] = StringBuilder(value); 
	}
	
	void append_param()
	{
		_params.push_back(_pack_item);
		_pack_item.clear();
	}
		
	template<class Session>	
	void Push(const std::string& ip, const int port)
	{
		typename Session::session_ptr_t ss =Session::instance().get(ip, port);
		typename pack_t::pack_ptr_t  pck(new pack_t(_name, _view, "push") );
		pck->type("push");
		for(auto &i:_params)
				pck->append_param(i);
		
		_params.clear();
		if(ss!=nullptr)
			ss->PostOutput(pck);
	}
	
				
protected:
	std::string _view;
	typename pack_t::param_item_t _pack_item;;
	params_pack_t _params;
	std::string _name;
	//pack_list_t _pcks;
	//std::string _src;
	//std::string _target;
};

NAMESP_END;

#endif /*RRESPONSE_H*/
