//type_convert.h

#ifndef TYPE_CONVERT_H
#define TYPE_CONVERT_H


#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#include <string>
#include <sstream>

NAMESP_BEGIN

template<class DES>
struct TypeConvertor
{
	template<class SRC>
	static DES Convert(SRC &src)
	{
		return Handle<DES,SRC>::Convert(src);
	}

	template<class HDES, class SRC>
	struct Handle;
	
};

template<>
template<>
struct TypeConvertor<int>::Handle<int, std::string>
{
	static int Convert(std::string &src)
	{
		int r;
		std::stringstream ss;
		ss << src;
		ss >> r;
		return std::move(r);
	}
};

template<>
template<>
struct TypeConvertor<size_t>::Handle<size_t, std::string>
{
	static size_t Convert(std::string &src)
	{
		size_t r;
		std::stringstream ss;
		ss << src;
		ss >> r;
		return std::move(r);
	}
};

template<>
template<>
struct TypeConvertor<float>::Handle<float, std::string>
{
	static float Convert(std::string &src)
	{
		float r;
		std::stringstream ss;
		ss << src;
		ss >> r;
		return std::move(r);
	}
};

template<>
template<>
struct TypeConvertor<std::string>::Handle<std::string, std::string>
{
	static std::string Convert(std::string &src)
	{
		return src;
	}
};

NAMESP_END

#endif /*TYPE_CONVERT_H*/
