//generic_factory.h
//copyright	: Copyright (c) 2017 arsee.
//license	: base on LGPL.
//author	: arsee

#ifndef GENERIC_FACTORY_H
#define GENERIC_FACTORY_H

#include <map>

/////////////////////////////////////////////////////////////
///@ class TFactory
template<class T, class TypeIdentifier, class Initializer>
class TFactory
{
        typedef TFactory<T, TypeIdentifier, Initializer> my_t;

public:
        typedef std::map<TypeIdentifier, my_t*> f_map_t;

public:
	static my_t& instance(){
		static my_t self;
		self.init();
		return self;
	}

        T* create(const TypeIdentifier& type){
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

template<class T,class TypeIdentifier, class Initializer>
inline void TFactory<T, TypeIdentifier, Initializer>::init(){
	if(_factoris.size() == 0)
		_factoris = Initializer()();
}


template<class T,class TypeIdentifier, class Initializer>
inline TFactory<T, TypeIdentifier, Initializer>::~TFactory(){
	typename f_map_t::iterator i = _factoris.begin();
	for(; i!=_factoris.end(); ++i)
		delete i->second;
}


///////////////////////////////////////////
template<class T, class Base, class TypeIdentifier, class Initializer>
class TCreator: public TFactory<Base, TypeIdentifier, Initializer>
{
public:
	virtual Base* create()
	{
		T* t = new T();
		return t;
	}
};

#endif /*GENERIC_FACTORY_H*/
