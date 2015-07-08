//collection.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-15
//record 
//****************************

#ifndef COLLECTION_H
#define COLLECTION_H

#include <algorithm>
#include <memory>
#include <map>

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif



NAMESP_BEGIN

///////////////////////////////////////////
//Collection
template<class ObjBase, class...OBJS>
class Collection
{
	typedef Collection<ObjBase, OBJS...> my_t;
	
public:	
	static my_t& instance() 
	{ 
		static my_t myselft;
		return myselft; 
	}
	
	typedef std::shared_ptr<ObjBase> obj_ptr_t;
	obj_ptr_t Get(const std::string& name){ return _objs[name]; }
	obj_ptr_t Get(std::string&& name){ return Get(name); }
	
private:
	Collection()
	{
		Iteration<Count, Put>::Handle(_objs);
	}
	
	typedef std::map<std::string, obj_ptr_t> map_t;
	
	
	enum{ Count=ArgCounter<OBJS...>::value};

	map_t _objs;
	
	template<int N>
	struct Put{
		void operator()(map_t& collt)
		{
			typedef typename ArgAt<N,OBJS...>::result obj_t;
			collt[obj_t::name()] = obj_ptr_t( new obj_t );
		}				
	};	

};

NAMESP_END;

#endif /*COLLECTION_H*/
