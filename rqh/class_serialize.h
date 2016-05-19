//serailize_class_pattern.h
//copyright	: Copyright (c) 2015 arsee.
//license	: GNU GPL v2.
//author	: arsee

#ifndef CLASS_SERAILIZE_H
#define CLASS_SERAILIZE_H

#include <string>
#include <string.h>
#include <memory>

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

#ifndef CLASS_INFO_BASE_H
#include "class_info_base.h"
#endif

NAMESP_BEGIN

template< class T>
struct ClassSerializer 
{
	template<class S>
	std::string operator()(S& s, T& obj)
	{
		std::string str = string("{\"className\":\"") + obj.get_class_info()->get_class_name()+"\","; 
		ClassInfoBase::attr_iterator i= obj.get_class_info()->attrs.begin();
		for(; i!=obj.get_class_info()->attrs.end(); ++i)
		{
			void* buf = i->second->get();
			if(i == --(obj.get_class_info()->attrs.end()))
				str += "\""+ i->first+"\":"
				    +s(i->second->typeof_(),buf,i->second->sizeof_());
			else
				str += "\""+ i->first+"\":"
				    +s(i->second->typeof_(),buf,i->second->sizeof_())+",";
		}
		str += "}";

		return str;
	}
};

template<>
struct ClassSerializer<int> 
{
	template<class S>
	std::string operator()(S& s, int& obj)
	{
		std::string str = string("{\"className\":\"");
		str += Typeof<int>()()+"\","; 
		str += s(Typeof<int>()() ,(void*)&obj, sizeof(int));
		str += "}";

		return str;
	}
};

class SerialExcept : public std::exception
{
	const char* what()const noexcept override
	{
		return "Serialilze failed:";
	}
};

template<class T>
struct ClassUnserialize
{
	template<class US, class ValueSchema>
	T* operator()(US& us, ValueSchema& schema) 
	{
		std::string class_name = schema["className"].asString();
		T* obj = (T*)ClassInfoBase::get_object(class_name.c_str());
		if (obj == nullptr)
			return nullptr;
			
		ClassInfoBase::attr_map_t::iterator i = obj->get_class_info()->attrs.begin();
		for(; i!=obj->get_class_info()->attrs.end(); ++i)
		{
			void* val = us(schema[i->first], i->first, i->second->typeof_()
				,i->second->sizeof_()); 
	
			if(val == nullptr)
			{
				delete obj;
				obj = nullptr;
				break;
			}
			i->second->set((const char*)val);
			//delete[] (char*)val;
		}
		
		return obj;
	}
};

template<>
struct ClassUnserialize<int>
{
	template<class US, class ValueSchema>
	int* operator()(US& us, ValueSchema& schema) 
	{
		std::string class_name = schema["className"].asString();
		int* obj = (int*) us(schema["val"], "val", class_name.c_str(), sizeof(int));
		return obj;
	}
};

NAMESP_END/*namespace*/

#endif /*CLASS_SERAILIZE_H*/
