//object.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-15
//record 
//****************************

#ifndef OBJECT_H
#define OBJECT_H

#include<list>
#include<algorithm>
#include <memory>

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#include <utility>

NAMESP_BEGIN


//@SOURCE the data object 
template<class SOURCE>
class Object//:
	//public IObject
{
	typedef SOURCE source_t;
	
public:
	Object()
		:_src()
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
	
	//const std::string Name(){ return std::move( _src.Name() ); }
	
	source_t& ref(){ return _src; }

	const source_t& ref()const { return _src; }

	static const std::string name(){ return source_t::name();}
	
private:
	source_t _src;	
	//std::unique_ptr<IResponse>  _rsp = nullptr; 
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
	void GetObj(std::string &name, Invoker &func, Args&... args)
	{
		Iteration<Count, GetObjT>::Handle(name, func, args...);
	}
	
	template<class Obj>
	Obj* GetObj()
	{
		ObjHolder<Obj> h;
		const std::string& name = Obj::name();
		Iteration<Count, GetObjT>::Handle(const_cast<std::string&>(name), h);
		return h.r;
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
	
	template<class ReturnObj>
	struct ObjHolder
	{
		ReturnObj* r;
		
		template<class FromObj>
		void Execute(FromObj* f){ r = (ReturnObj*)(f); }
	};

	template<int N>
	struct GetObjT
	{
		//template<class Invoker, class... Args>
		//auto operator()(std::string &name, Invoker &func, Args&... args)
		//->decltype(std::get<N>( my_t::Instance()._objs ))
		//{
		//	if ( std::get<N>( my_t::Instance()._objs )->Name() == name )
		//		return std::get<N>(my_t::Instance()._objs);			
		//}
		
		template<class Invoker, class... Args>
		void operator()(std::string &name, Invoker &func, Args&... args)
		{
			
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

#endif /*OBJECT_H*/
