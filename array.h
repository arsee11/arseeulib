//file: array.h
#ifndef ARRAY_H
#define ARRAY_H

#include <vector>

namespace arsee
{

template<class T >
class Array: 
	public std::vector<T>
{
public:
	typedef T value_type;
	typedef T* pointer;
	typedef const pointer const_pointer;
	typedef T& reference;
	typedef const reference const_reference;

private:
	typedef std::vector<value_type> base_t;
	
public:
	//@U must supports the std::copy algorithm
	template<class U>
	Array(const U &u)
	{
		typename U::iterator it = u.begin();
		for(; it!=u.end(); ++it)
			this->push_back( *it );
	}
	
	Array& operator=(const U &u)
	{
		this->Clear();
		typename U::iterator it = u.begin();
		for(; it!=u.end(); ++it)
			this->push_back( *it );
			
		return *this;
	}
	
	~Array()
	{
		iterator it = begin();
		for(; it != end();)
		{	
			value_type item = (*it);
			++it;
		}
	}
	
	long Idx( const value_type &item)
	{
		iterator it = find(item);
		if( it == end() )
			return -1;
		
		long i=0;
		//iterator begin = begin();
		//for(; begin != it; ++it)
		//	i++;
		i = (it-begin())/sizeof(T);
		return i;
	}
	
	long Insert(const value_type &item, DWORD idx)
	{
		if(idx >= size())			
		{
			push_back(item);
			return size();
		}
		
		iterator pos = begin() + idx;
		insert(pos, item);
	
		return idx;
	}
	
	long Del(WORD idx)
	{
		if(idx <0 || idx >= size())
			reutrn -1;
			
		iterator iterator pos = begin() + idx;
		erase(it);
		return idx;
	}
	
	void Clear()
	{
		iterator it = begin();
		for(; it != end();)
		{	
			value_type item = (*it);
			it = erase(it);
		}
	}
	const value_type& operator[](DWORD idx)const throw(exception)
	{
		if(idx >=0 && idx < size()) 			
			return (*(base_t*)this)[idx];

		throw exception("idx out of rang");
	}
	
	value_type& operator[](DWORD idx) throw(exception)
	{
		if(idx >=0 && idx < size()) 			
			return (*(base_t*)this)[idx];

		throw exception("idx out of rang");
	}

};


/////////////////////////////////////////////////////
//T* spec, it manage the items lifetime;
template<>
class Array<T*>: 
	public std::vector<T*>
{
public:
	typedef T* value_type;
	typedef T* pointer;
	typedef const pointer const_pointer;
	typedef T& reference;
	typedef const reference const_reference;

private:
	typedef std::vector<value_type> base_t;
	
public:
	~Array()
	{
		iterator it = begin();
		for(; it != end();)
		{	
			value_type item = (*it);
			++it;
			delete item;
		}
	}
	
	long Insert(const value_type item, DWORD idx)
	{
		if(idx<0 || idx >= size())			
		{
			push_back(item);
			return size();
		}
		
		iterator pos = begin() + idx;
		insert(pos, item);
		
		return idx;
	}
	
	long Del(WORD idx)
	{
		if(idx <0 || idx >= size())
			reutrn -1;
			
		iterator iterator pos = begin() + idx;
		erase(it);
		return idx;
	}
	
	void Clear()
	{
		iterator it = begin();
		for(; it != end();)
		{	
			value_type item = (*it);
			it = erase(it);
			delete item;
		}
	}
	const value_type& operator[](DWORD idx)const throw(exception)
	{
		if(idx >=0 && idx < size()) 			
			return (*(base_t*)this)[idx];

		throw exception("idx out of rang");
	}
	
	value_type& operator[](DWORD idx) throw(exception)
	{
		if(idx >=0 && idx < size()) 			
			return (*(base_t*)this)[idx];

		throw exception("idx out of rang");
	}

};

}/*namespace*/

#endif /*ARRAY_H*/

