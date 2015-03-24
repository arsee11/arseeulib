#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <sstream>


#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

using namespace std;

NAMESP_BEGIN

template<class T>
T::value_type sum(T &array)
{
	T::value_type sum = 0;
	T::iterator it = array.begin();
	for(; it != array.end(); ++it)
		sum += (*it);

	return sum;
}

template<class T>
T string2t(string &value, T &out)
{	
	strstream ss;
	ss<<value;
	ss>>out;
	return  out;
}

template<class T>
string t2string(T value)
{	
	strstream ss;
	ss<<value;
	string str;
	ss>>str;
	return str;
}

template<class ITERATOR, class COMPARE>
bool issame(ITERATOR start, ITERATOR end, COMPARE &cmp  )
{
	for(; start != end; ++start)
	{
		cmp.Set( (*start) );
		ITERATOR init = start;
		init++;
		if(init == end )
			break;

		if( find_if(init, end, cmp) == end )
			return false;
	}

	return true;
}

template<class ITERATOR>
bool issame(ITERATOR &start, ITERATOR &end )
{
	ITERATOR init = start;

	for(; start != end; ++start)
	{
		if( find(init, end, (*start) )!= end )
			return false;
	}

	return true;

}

NAMESP_END/*namespace*/

#endif /*UTILITY_H*/
