//class_creation_reactor.h
//copyright	: Copyright (c) 2015 arsee.
//license	: GNU GPL v2.
//author	: arsee

#ifndef CLASS_CREATION_REACTOR_H
#define CLASS_CREATION_REACTOR_H

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


NAMESP_END/*namespace*/

#endif /*CLASS_CREATION_REACTOR_H*/
