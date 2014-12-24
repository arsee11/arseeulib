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


//@SOURCE the data object 
template<class SOURCE, class RspImpl>
class Object//:
	//public IObject
{
	typedef SOURCE source_t;
public:
	typedef typename RspImpl::view_t view_t;
	
public:
	Object()
		:_src()
		,_rsp("")
	{
	}
	
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
	
	template<class Sender> 
	int Update(const Sender& ss)
	{
		//_rsp->Push(param);
			
		return 0;
	}
	
	const std::string Name(){ return std::move( _src.Name() ); }
	
	int Attach( const view_t &view)
	{
		_rsp.AttachView(view);
	}
	
	void Attach( const view_t &&view)
	{
		Attach(view);
	}
	
	void Detach( const view_t &view)
	{
		_rsp.DetachView(view);
	}
	
	source_t& ref(){ return _src; }

	const source_t& ref()const { return _src; }

	static const std::string name(){ return source_t::name();}
	
private:
	source_t _src;	
	//std::unique_ptr<IResponse>  _rsp = nullptr; 
	RspImpl _rsp;
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
	
	template<class Invoker, class... Args >
	static void GetObj(std::string &name, Invoker &func, Args&... args)
	{
		Iteration<Count, GetObjT>::Handle(name, func, args...);
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
		template<class Invoker, class... Args>
		void operator()(std::string &name, Invoker &func, Args&... args)
		{
			//if ( std::get<N>( my_t::Instance()._objs )->Name() == name )
			//	acc.AttachSrc( std::get<N>(my_t::Instance()._objs) );
				
			if ( ArgAt<N, OBJS...>::result::name() == name )
				func.Execute(std::get<N>( my_t::Instance()._objs ), args...);
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
//����
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
