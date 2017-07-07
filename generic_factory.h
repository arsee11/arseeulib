//generic_factory.h
//copyright	: Copyright (c) 2017 arsee.
//license	: GNU GPL v2.
//author	: arsee



#ifndef GENERIC_FACTORY_H
#define GENERIC_FACTORY_H

#include <map>
#include <string>

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN

/////////////////////////////////////////////////////////////
///@ class TFactory
template<class T, class Initializer>
class TFactory
{
	typedef TFactory<T, Initializer> my_t;

public:
	typedef std::map<std::string, my_t*> f_map_t;

public:
	static my_t& instance(){
		static my_t self;
		self.init();
		return self;
	}

	T* create(const std::string& type){
		typename f_map_t::iterator i;
		if( (i=_factoris.find(type)) != _factoris.end()	)
		{
			return i->second->create();
		}
		
		return NULL;
	}

	TFactory(){}
	TFactory(const my_t&);
	~TFactory();
private:
		
	virtual T* create(){ return NULL;}
	
	void init();
private:
	f_map_t _factoris;
};

template<class T, class Initializer>
inline void TFactory<T, Initializer>::init(){
	if(_factoris.size() == 0)
		_factoris = Initializer()();
}


template<class T, class Initializer>
inline TFactory<T, Initializer>::~TFactory(){
	typename f_map_t::iterator i = _factoris.begin();
	for(; i!=_factoris.end(); ++i)
		delete i->second;
}


///////////////////////////////////////////
template<class T, class Base, class Initializer>
class TCreator: public TFactory<Base, Initializer>
{
public:
	virtual Base* create()
	{
		T* t = new T();
		return t;
	}
};

NAMESP_END

#endif /*GENERIC_FACTORY_H*/
