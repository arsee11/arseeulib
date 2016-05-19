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

#ifndef RRESPONSE_H
#include "rresponse.h"
#endif

#include <iostream>
#include <exception>


NAMESP_BEGIN

//exceptions
class dispatcher_notfound_exp : public std::exception
{
	public:
	const char* what()const noexcept override
	{
		return msg;
	}

	const char* msg = "dispatcher_notfound_exp";
};



///////////////////////////////////////////////////////////////////
//IDispatcher
template<class Pack>
class IDispatcher
{
public:
	typedef Pack pack_t;
	typedef typename pack_t::pack_list_t pack_list_t;
	
	virtual pack_list_t Execute(RequestContext& context, pack_t &pck) throw(dispatcher_notfound_exp)=0;
};



/////////////////////////////////////////////////
///Dispatcher
template<class CONTROL>
class Dispatcher : public IDispatcher<typename CONTROL::pack_t>
{
public:
	typedef CONTROL control_t;
	typedef IDispatcher<typename CONTROL::pack_t> base_t;

	typedef typename base_t::pack_t pack_t;
	typedef typename base_t::pack_list_t pack_list_t;

public:
	static std::string name(){ return control_t::rqt_name(); }
	
	//
	pack_list_t Execute(RequestContext& context, pack_t &pck) throw(dispatcher_notfound_exp) override
	{
		pack_t& cont_pack = _context2pack_map[context.id_str()];
		pack_list_t replies;
		if( !pck.get_continue().empty() )
		{
			if(!cont_pack.status())
				cont_pack = pck;
			else
				cont_pack += pck;
		}
		else
		{
			control_t ctrl(pck.source(), &context);
			ctrl.Request(pck);
			ctrl.Reply(replies);
		}
		
		if( pck.get_continue() == "next" )
		{
			RResponse<pack_t> rsp("response");
			//rsp.Success();
			replies.push_back( typename pack_t::pack_ptr_t(rsp.Reply()) );
		}
		else if( pck.get_continue() == "end" )
		{
			control_t ctrl(cont_pack.source(), &context);
			ctrl.Request(cont_pack);
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
	
public:
	template<class Pack>
	static typename Pack::pack_list_t Handle(RequestContext& context
		,Pack &pck
	) throw(dispatcher_notfound_exp)
	{
		typedef IDispatcher<Pack> dispatcher_t;
		typedef Collection<dispatcher_t, ConcreteDispathers...> collection_t;

		typename collection_t::obj_ptr_t disp = collection_t::instance().Get( pck.action() );
		if(disp == nullptr)
			throw dispatcher_notfound_exp();
			
		return disp->Execute(context, pck);
	}
	
};

NAMESP_END

#endif /*DISPATCHER_H*/
