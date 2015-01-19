//stringex.h

#ifndef STRINGEX_H
#define STRINGEX_H

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

#include <sstream>
#include <string>
#include <utility>

using namespace std;

NAMESP_BEGIN

template<class T>
inline string StringBuilder(const T &val)
{
	stringstream ss;
	ss<<val;
	string str;
	ss>>str;
	return str;
}

template<class T>
inline string StringBuilder(T &&val)
{
	return StringBuilder(val);
}

inline string StringBuilder(const string &val)
{
	return val;
}

inline string StringBuilder(const char* val)
{
	return std::move(string(val));
}

template<class T>
inline T string2t(string &value)
{	
	T out;
	stringstream ss;
	ss<<value;
	ss>>out;
	return  out;
}


NAMESP_END

#endif /*STRINGEX_H*/
