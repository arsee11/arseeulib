//class_creation_reactor.h
//copyright	: Copyright (c) 2015 arsee.
//license	: GNU GPL v2.
//author	: arsee

#ifndef CLSS_CREATION_REACTOR_H
#define CLSS_CREATION_REACTOR_H

#include <map>
#include <string>
#include <iostream>

using namespace std;

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

NAMESP_BEGIN

typedef void*(*Creator)(void);

class ClassFactory
{
public:
	typedef std::map<std::string, Creator> creation_map_t;

	static ClassFactory& instance()
	{
		static ClassFactory _myselft;
		return _myselft;
	}
	
	void* Create(const char* name)
	{
		creation_map_t::iterator i = _maps.find(name);
		if( i != _maps.end() )
			return (i->second)();
		
		return nullptr;
	}

	void Registry(const char* class_name, Creator creator)
	{
		std::string name = class_name;
		_maps[name] = creator;
	}

private:
	
	creation_map_t _maps;

	
};


template<class T, const char* class_name> 
class Register
{
public:
	Register()
	{
		cout<< "Register()"<<endl;
		ClassFactory::instance().Registry(class_name, NewT);
	}
	
	static void* NewT(){ return new T; }
};

NAMESP_END/*namespace*/

#endif /*CLSS_CREATION_REACTOR_H*/
