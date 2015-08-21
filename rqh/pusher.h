//pusher.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee



#ifndef PUSHER_H
#define PUSHER_H

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


/////////////////////////////////////////////////
//Pusher
template<class Pack>
class Pusher
{
public:
	typedef Pack pack_t;
	typedef typename pack_t::params_pack_t 	params_pack_t;
	typedef typename pack_t::pack_ptr_t  	pack_ptr_t;
	typedef typename pack_t::pack_list_t 	pack_list_t;
	
public:
	Pusher(const std::string& name)
		:_name(name)
	{
	}

	Pusher(std::string&& name)
		:Pusher(name)
	{
	}
	
	Pusher(const char* name)
		:Pusher(std::string(name))
	{
	}
	
	
	Pusher()
		:Pusher("pusher") 
	{
	}
	
		
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
		typename pack_t::pack_ptr_t  pck(new pack_t(_name, "push") );
		for(auto &i:_params)
				pck->append_param(i);
		
		_params.clear();
		if(ss!=nullptr)
			ss->PostOutput(pck);
	}
	
				
protected:
	typename pack_t::param_item_t _pack_item;;
	params_pack_t _params;
	std::string _name;
};


template<class Pusher>
class MultPusher
{
public:
	static Pusher* Begin()
	{
		
	}
	
	static Pusher* Next();
	static Pusher* End();
};


NAMESP_END;

#endif /*PUSHER_H*/
