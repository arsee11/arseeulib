//dispatcher.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-15
//record 
//****************************

#ifndef DISPATCHER_H
#define DISPATCHER_H

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef COLLECTION_H
#include "collection.h"
#endif

#include <iostream>
#include <exception>


NAMESP_BEGIN

//exceptions
calss dispatcher_notfound_exp : public std::exception
{
};



///////////////////////////////////////////////////////////////////
//IDispatcher
template<class ObjectsCollection, class Pack>
class IDispatcher
{
public:
	typedef Pack pack_t;
	typedef typename pack_t::pack_list_t pack_list_t;
	typedef ObjectsCollection objcollection_t;
	
	virtual pack_list_t Execute(RequestContext& context, ObjectsCollection &objs, pack_t &pck) throw(dispatcher_notfound_exp)=0;
};



/////////////////////////////////////////////////
///Dispatcher
template<class CONTROL, class RequestContext, class ObjectsCollection>
class Dispatcher : public IDispatcher<RequestContext, ObjectsCollection, typename CONTROL::pack_t>
{
public:
	typedef CONTROL control_t;
	typedef typename control_t::obj_t obj_t;

public:
	static std::string name(){ return control_t::rqt_name(); }
	
	//
	pack_list_t Execute(RequestContext& context, ObjectsCollection &objs, pack_t &pck) override
	{
		pack_t& cont_pack = _context2pack_map[context.id_str()];
		pack_list_t replies;
		if( !pack.get_continue().empty() )
		{
			cont_pack += pack;
		}
		else
		{
			control_t ctrl(pack.source(), &context);
			obj_t* obj = objs.template GetObj<obj_t>();
			ctrl.Request(obj, pack);
			ctrl.Reply(replies);
		}
		
		if( pack.get_continue() == "next" )
		{
			RResponse rsp;
			rsp.add_param("code", 0);
			rsp.append_param();
			replies.push_back( pack_ptr_t(rsp.Reply()) );
		}
		else if( pack.get_continue() == "end" )
		{
			control_t ctrl(cont_pack.source(), &context);
			obj_t* obj = objs.template GetObj<obj_t>();
			ctrl.Request(obj, cont_pack);
			ctrl.Reply(replies);
			cont_pack.Reset();
		}

		return std::move(replies);
	}

private:
	std::map<std::string, pack_t> _context2pack_map;
};


template<class... ConcreteDispathers>
class DispatcherHandler
{
	typedef Collection<IDispatcher, ConcreteDispathers> collection_t;
	
public:
	static IDispatcher::pack_list_t Handle(RequestContext& context
		   ,IDispatcher::objcollection_t &objs
		   ,IDispatcher::pack_t &pck
	) throw(dispatcher_notfound_exp)
	{
		typename collection_t::obj_ptr_t disp = collection_t::instance().Get( pck.action() );
		if(disp == nullptr)
			throw dispatcher_notfound_exp();
			
		return disp->Execute(context, objs, pck);
	}
	
};

NAMESP_END

#endif /*DISPATCHER_H*/
