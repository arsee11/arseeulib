#ifndef UTILITYS_H
#define UTILITYS_H

#include <string>
#include <vector>
#include <sstream>


#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

using namespace std;

NAMESP_BEGIN

template<class T>
typename T::value_type sum(T &array)
{
	typename T::value_type sum = 0;
	typename T::iterator it = array.begin();
	for(; it != array.end(); ++it)
		sum += (*it);

	return sum;
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

template<class T, template<class, class> class V>
struct IsVector
{
	typedef char ONE;
	typedef struct{char c[2];} TWO;
	T t;
	
	template<class U>
	static ONE is_vector(V<U, allocator<U> > u);
	template<class U>
	static TWO is_vector(U u);
	enum{ isVector=sizeof(IsVector<T, V>::is_vector(t))};
};

template<class T>
struct IsSharedPtr
{
	typedef char ONE;
	typedef struct{char c[2];} TWO;
	T t;
	
	template<class U>
	static ONE is_ptr(shared_ptr<U> u);
	template<class U>
	static TWO is_ptr(U u);
	enum{ isSharedPtr=sizeof(IsSharedPtr<T>::is_ptr(t))};
};

///////////////////////////////////////////////
//IfElseThen Begin
template<bool pre, class A, class B>
struct IfElseThen;

template<class A, class B>
struct IfElseThen<true, A, B>
{
	typedef A value;
};

template<class A, class B>
struct IfElseThen<false, A, B>
{
	typedef B value;
};
//IfElseThen End
///////////////////////////////////////////////

NAMESP_END/*namespace*/

#endif /*UTILITYS_H*/
