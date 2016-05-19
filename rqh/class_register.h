//class_register.h
//copyright	: Copyright (c) 2015 arsee.
//license	: GNU GPL v2.
//author	: arsee

#ifndef CLASS_REGISTER_H
#define CLASS_REGISTER_H

#include <iostream>

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef CLASS_SERAILIZE_HANDLER_H
#include "class_serialize_handler.h"
#endif

#ifndef CLASS_CREATION_REACTOR_H
#include "class_creation_reactor.h"
#endif

NAMESP_BEGIN

template<class T, const char* class_name> 
class Register
{
public:
	Register()
	{
//		cout<< "Register()"<<endl;
		ClassFactory::instance().Registry(class_name, NewT);
		SerializeHandler::instance().Registry(class_name, Serialize);
		SerializeHandler::instance().RegistryUs(class_name, new Us<T>);
	}
	
	static void* NewT(){ return new T; }
	static std::string Serialize(void* memory, size_t size)
	{
		return Coder<T>().code(memory,size);
	}
}; 

template<class T>
class RegisterVector
{
public:
	RegisterVector()
	{
		SerializeHandler::instance().Registry(Typeof<T>()().c_str(), Serialize);
		SerializeHandler::instance().RegistryUs(Typeof<T>()().c_str(), new Us<T>);
	}
	
	static std::string Serialize(void* memory, size_t size)
	{
		return SerializeVector( *(T*)memory, size); 
	}
}; 

template<class T>
class RegisterSharedPtr
{
public:
	RegisterSharedPtr()
	{
		SerializeHandler::instance().Registry(Typeof<T>()().c_str(), Serialize);
		SerializeHandler::instance().RegistryUs(Typeof<T>()().c_str(), new Us<T>);
	}
	
	static std::string Serialize(void* memory, size_t size)
	{
		return Coder<T>().code( memory, size); 
	}
}; 


NAMESP_END/*namespace*/

#endif /*CLASS_REGISTER_H*/
