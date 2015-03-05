//stringex.h

#ifndef STRINGEX_H
#define STRINGEX_H

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

#include <sstream>
#include <string>
#include <utility>
#include <locale.h>


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
inline T str2t(const string &value)
{	
	T out;
	stringstream ss;
	ss<<value;
	ss>>out;
	return  out;
}

template<class T>
string t2str(T value)
{
	stringstream ss;
	ss.clear();
	ss << value;
	string str;
	ss >> str;
	return str;
}


//string转wstring
inline std::wstring str2wstr(const std::string &str)
{
	if(str.empty())
		return L"";

    unsigned len = str.size() * 2;
    setlocale(LC_CTYPE, "");     //必须调用此函数
    wchar_t *p = new wchar_t[len];
    mbstowcs(p,str.c_str(),len);
    std::wstring str1(p);
    delete[] p;
    return str1;
}

// wstring转string
inline std::string wstr2str(const std::wstring &str)
{
	if( str.empty() )
		return "";

    unsigned len = str.size() * 4;
    setlocale(LC_CTYPE, "");
    char *p = new char[len];
    wcstombs(p,str.c_str(),len);
    std::string str1(p);
    delete[] p;
    return str1;
}

NAMESP_END

#endif /*STRINGEX_H*/
