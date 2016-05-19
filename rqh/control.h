//control.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

#ifndef CONTROL_H
#define CONTROL_H

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef RESPONSE_H
#include "rresponse.h"
#endif

#ifndef REQUEST_H
#include "request.h"
#endif

#ifndef REQUEST_CONTEXT_H
#include "request_context.h"
#endif

NAMESP_BEGIN


//protocol 
//request:target.request.parameter
//{"target"="target","request"="request", [{"param"=p1},{"param"=p2"}]}
//response:target.response.source.parameter
//{"source"="src","respone"="response", [{"target"="t1"},{"target"="t2"}],[{"param"=p1},{"param"=p2"}]}
//transmission
//A.
//1.source->(request)->model
//2.model->(response)->source
//B.
//
//

//////////////////////////////////////////////
//Remote Control.
//Inter-proccess.
//@PACK inter pack.
//@Whoes the obj who send the pack here.
//@Logic request method.
/*template<class Pack 
	,class Logic
>
class RControl
{

public:
	typedef Pack pack_t;
	typedef typename pack_t::params_pack_t 	params_pack_t;
	typedef typename pack_t::pack_ptr_t  	pack_ptr_t;
	typedef typename pack_t::pack_list_t 	pack_list_t;
	
	typedef Logic logic_t;
	typedef typename logic_t::obj_t obj_t;
	typedef RRequest<logic_t> request_t;
	typedef IRResponse<pack_t> response_t;
	
	
	const static std::string rqt_name() { return logic_t::name(); }
	const static std::string target(){ return logic_t::target(); }
	
	RControl(std::string& source, RequestContext* context)
		:_view(source)
	{
		_rqt.AttachContext(context);
		_rqt.AttachLogic( new logic_t() );
	}

	RControl(std::string&& source, RequestContext* context)
		:RControl(source, context)
	{}	
	
	bool Request(obj_t* obj, const pack_t& pck) throw(std::exception)
	{
		const typename pack_t::params_pack_t &params = pck.params();
		for(auto &i:params)
		{
			_rsp = unique_ptr<response_t>( _rqt.Execute(obj, i ) );
			if(_rsp == nullptr)
				throw std::exception("_rsp==nullptr");

			_rsp->source(_view);
			_rsp->action( pck.action() );
		}
		
		return true;
	}
	
	void Reply(pack_list_t& pcks)
	{
		if( _rsp == nullptr )
			return;

		pack_ptr_t pck( _rsp->Reply() );
		if(pck!=nullptr && pck->status())
			pcks.push_back(pck);
	}

private:
	request_t  _rqt;
	unique_ptr<response_t> _rsp = nullptr;
	int _state = 0;
	std::string _view;
};
*/
//////////////////////////////////////////////
//Remote Control for ObjectLogic.
//Inter-proccess.
//@PACK inter pack.
//@Whoes the obj who send the pack here.
//@Logic request method.
template<class Pack, class Logic>
class RControl
{

public:
	typedef Pack pack_t;
	typedef typename pack_t::pack_ptr_t  	pack_ptr_t;
	typedef typename pack_t::pack_list_t 	pack_list_t;
	
	typedef Logic logic_t;
	typedef RRequestObject<pack_t> request_t;
	typedef IRResponse<pack_t> response_t;
			
	const static std::string rqt_name() { return logic_t::name(); }
	const static std::string target(){ return logic_t::target(); }
	
	RControl(std::string& source, RequestContext* context=nullptr)
		:_source(source)
	{
		_rqt.AttachContext(context);
		_rqt.AttachLogic( new logic_t );
	}

	RControl(std::string&& source, RequestContext* context=nullptr)
		:RControl(source,  context)
	{}
	
	
	bool Request(const pack_t& pck) throw(std::exception)
	{
		_rsp = unique_ptr<response_t>( _rqt.Execute(pck) );
		if(_rsp == nullptr)
			throw std::exception();

		_rsp->view(_source);
		_rsp->action( pck.action() );		
		
		return true;
	}
	
	void Reply(pack_list_t& pcks)
	{
		if( _rsp == nullptr )
			return;

		pack_ptr_t pck( _rsp->Reply() );
		if(pck!=nullptr && pck->status())
			pcks.push_back(pck);
	}

private:
	request_t  _rqt;
	unique_ptr<response_t> _rsp = nullptr;
	int _state = 0;
	std::string _source;
};

NAMESP_END;

#endif /*CONTROL_H*/
