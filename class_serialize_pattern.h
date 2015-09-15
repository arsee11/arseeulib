//serailize_class_pattern.h
//copyright	: Copyright (c) 2015 arsee.
//license	: GNU GPL v2.
//author	: arsee

#ifndef CLASS_SERAILIZE_PATTERN_H
#define CLASS_SERAILIZE_PATTERN_H

#include <string>
#include <string.h>

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

#include "class_info.h"

NAMESP_BEGIN

std::string code(const char* buf, int size)
{
	if(size == 0 )
		return buf;

	std::string str;
	for (int i = 0; i < size; ++i)
	{
		char ch[8] = { 0 };
		sprintf(ch, "%02x", (unsigned char)buf[i]);
		str += ch;		
	}

	return str;
}


int xtoi(char xch)
{
	if (xch == 'A' || xch == 'a')
		return 10;

	if (xch == 'B' || xch == 'b')
		return 11;

	if (xch == 'C' || xch == 'c')
		return 12;

	if (xch == 'D' || xch == 'd')
		return 13;

	if (xch == 'E' || xch == 'e')
		return 14;

	if (xch == 'F' || xch == 'f')
		return 15;

	return atoi(&xch);
}

char atobx(const char byte[2])
{
	char high = xtoi(byte[0]);
	char low = xtoi(byte[1]);

	return high * 16 + low;
}

const char* encode(const std::string& str, int size)
{
	if(size == 0 )
		return str.c_str();
		
	char *buf = new char[size];
	memset(buf, 0, size);
	char byte[2] = { 0 };
	for (int i = 0, j=0; i < size*2; i++){		
		byte[i%2] = str[i];
		if (i % 2 != 0)
		{
			buf[j] = atobx(byte);
			++j;
		}
	}

	return buf;
}


template<class T>
std::string class_serialize(T& obj)
{
	std::string str = string("\"") + obj.class_info.get_class_name() + "\":{";
	ClassInfoBase::attr_iterator i= obj.class_info.attrs.begin();
	for(; i!=obj.class_info.attrs.end(); ++i)
	{
		shared_ptr<char> buf = i->second->get();
		str += "\""+ i->first+"\":\""+code(buf.get(), i->second->sizeof_())+"\",";
	}
	str += "}";

	return str;
}


//@return new object, must delete it manual.
template<class T, class attr_value_map>
T* class_unserialize(const std::string& class_name, const attr_value_map& attr)
{
	T *obj = (T*)ClassInfoBase::get_object(class_name.c_str());
	if (obj == nullptr)
		return nullptr;
		
	typename attr_value_map::const_iterator i= attr.begin();
	for(; i!=attr.end(); ++i)
	{
		ClassInfoBase::attr_iterator j = obj->class_info.attrs.find(i->first);
		if (j == obj->class_info.attrs.end())
		{
			delete obj;
			return nullptr;
		}
		
		j->second->set( encode(i->second, j->second->sizeof_()) );
	}
	
	return obj;
}


NAMESP_END/*namespace*/

#endif /*CLASS_SERAILIZE_PATTERN_H*/
