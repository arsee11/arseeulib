//file: mvcrobject.h

#ifndef MVC_OBJECT_H
#define MVC_OBJECT_H

#include<list>
#include "mvcresponse_abstr.h"
#include <memory>

#ifndef GLOBALDEF_H
#include "globaldef.h"
#endif

#include <xutility>

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
	
public:

	Object()
		:_src()
	{}
	
	template<class... Us>
	Object(Us*... us)
	{
		_src = std::move<source_t>(source_t(us...));
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
	
	int Update()
	{
		auto i = _rsplst.begin();

		for(;i!=_rsplst.end(); ++i)
			(*i)->Update();
			
		return 0;
	}
	
	std::string Name(){ return std::move<std::string>( _src.Name() ); }
	
	//own ther IReponse,manage it's lifetime. 
	int Attach( IResponse *rps)
	{
		_rsplst.push_back(std::unique_ptr<IResponse>(rps));
		return 0;
	}
	
	int Dettach( IResponse *rps)
	{
		auto it = find(_rsplst.begin(), _rsplst.end(), std::unique_ptr<IResponse>(rps) );
		if( it != _rsplst.end() )
			_rsplst.erase( it );
		return 0;
	}
	
	source_t& ref(){ return _src; }

	const source_t& ref()const { return _src; }
	
private:
	source_t _src;	
	std::list< std::unique_ptr<IResponse> > _rsplst;
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
			throw exception("Not Init!");
			
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
	
	template<class...OBJS>
	struct Trait
	{
		typedef std::tuple<OBJS*...> value;
		enum{ Count = ArgCounter<OBJS*...>::value };
	};
	
	typedef typename Trait<OBJS...>::value collection_t;
	
	template<int N>
	struct GetObjT
	{
		template<class ACCEPTER>
		void operator()(std::string &name, ACCEPTER &acc)
		{
			if ( std::get<N>( my_t::Instance()._objs )->Name() == name )
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
