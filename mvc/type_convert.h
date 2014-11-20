//type_convert.h

#ifndef TYPE_CONVERT_H
#define TYPE_CONVERT_H

#ifndef GLOBALDEF_H
#include "globaldef.h"
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

	template<class DES, class SRC>
	struct Handle;
	
	template<>
	struct Handle<int, std::string>
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
	struct Handle<float, std::string>
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
	struct Handle<std::string, std::string>
	{
		static std::string Convert(std::string &src)
		{
			return src;
		}
	};
};

NAMESP_END

#endif /*TYPE_CONVERT_H*/