//mvcrobject.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-15
//record 
//****************************

#ifndef MVC_OBJECT_H
#define MVC_OBJECT_H

#include<list>
#include<algorithm>
#include <memory>

#include "mvcresponse_abstr.h"

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#include <utility>

NAMESP_BEGIN

//class IObject
//{  
//public:
//	virtual int Update()=0;
//};

template<class T>
class RResponse;

//@SOURCE the data object 
template<class SOURCE>
class Object//:
	//public IObject
{
	typedef SOURCE source_t;
		
public:
	typedef RResponse<Object<source_t> > response_t;
	typedef response_t::view_t view_t;
	
public:
	Object()
		:_src()
	{}
	
	template<class... Us>
	Object(Us*... us)
	{
		_src = std::move(source_t(us...));
	}
	
	template<class... Us>
	Object(Us&&... us)
		:_src(us...)
	{}
	
	template<class... Us>
	Object(const Us&... us)
		:_src(us...)
	{}
	
	template<class... Us>
	Object(Us&... us)
		: _src(us...)
	{}

	Object &operator=(const Object& rhs)
	{
		if( this == &rhs )
			return (*this);
		
		(*this)._src = rhs._src;
		(*this)._rsplst = rhs._rsplst;
		
		return (*this);
	}
	
	template<class Param>
	int Update(const Param &param)
	{
		auto i = _rsplst.begin();

		for(;i!=_rsplst.end(); ++i)
			(*i)->Push(param);
			
		return 0;
	}
	
	const std::string Name(){ return std::move( _src.Name() ); }
	
	int Attach( const view_t &view)
	{
		_rsp->push_back(view);
	}
	
	void Attach( const view_t &&view)
	{
		Attach(view);
	}
	
	void Detach( const view_t &view)
	{
		_rsp->Detach(view);
	}
	
	source_t& ref(){ return _src; }

	const source_t& ref()const { return _src; }
	
private:
	source_t _src;	
	std::unique_ptr<IResponse> > _rsp = std::unique_ptr<IResponse> >( new response_t(this));
};


///////////////////////////////////////////
//ObjectsCollection
template<class...OBJS>
class ObjectsCollection
{
	typedef ObjectsCollection<OBJS...> my_t;
	
public:	
	static my_t& Instance() throw(std::exception)
	{ 
		if(_myself == nullptr) 
			throw std::exception();
			
		return (*_myself.get()); 
	}
	
	static void Init(OBJS*... objs)
	{
		if( _myself == nullptr )
			_myself = std::unique_ptr<my_t>( new my_t(objs...) );		
	}
	
	template<class ACCEPTER>
	static void GetObj(std::string &name, ACCEPTER &acc)
	{
		Iteration<Count, GetObjT>::Handle(name, acc);
	}
	
private:
	ObjectsCollection(OBJS*... objs)
	{
		_objs = std::make_tuple(objs...);
	}
	
	template<class...Args>
	struct Trait
	{
		typedef std::tuple<Args*...> value;
		enum{ Count = ArgCounter<Args*...>::value };
	};
	
	typedef typename Trait<OBJS...>::value collection_t;
	
	template<int N>
	struct GetObjT
	{
		template<class ACCEPTER>
		void operator()(std::string &name, ACCEPTER &acc)
		{
			//if ( std::get<N>( my_t::Instance()._objs )->Name() == name )
			//	acc.AttachSrc( std::get<N>(my_t::Instance()._objs) );
				
			if ( ArgAt<N, OBJS>::result::name() == name )
				acc.AttachSrc( std::get<N>(my_t::Instance()._objs) );
		}
	};	
	

	enum{ Count=Trait<OBJS...>::Count};

	collection_t _objs;
	
private:
	//collection_t _objs;
	static std::unique_ptr<my_t> _myself;
};

template<class...OBJS>
std::unique_ptr<ObjectsCollection<OBJS...> > ObjectsCollection<OBJS...>::_myself;


///////////////////////////////
//ÄÑÌâ
class DataMemberGetter
{
public:
	template<class OBJ, class T> 
	void Solve(OBJ *obj, T &t)
	{
	}
	
	template<class OBJ, class... T> 
	void Solve(OBJ *obj, T&... t)
	{
		
	}
};

class DataMemberSetter
{
};

NAMESP_END;

#endif /*MVC_OBJECT_H*/
