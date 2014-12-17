//mvccontrol.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-15
//record 
//****************************

#ifndef MVC_CONTROL_H
#define MVC_CONTROL_H

#ifndef MVC_OBJECT_H
#include "mvcobject.h"
#endif

#ifndef GLOBALDEF_H
#include "globaldef.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef MVC_RESPONSE_H
#include "mvcresponse.h"
#endif

#ifndef MVC_REQUEST_H
#include "mvcrequest.h"
#endif

#ifndef TYPE_TRANSFOR_H
//#include "type_transfor.h"
#endif

NAMESP_BEGIN


//protocol 
//request:target.request.parameter
//{"target"="target","request"="request", [{"param"=p1},{"param"=p2"}]}
//response:target.response.source.parameter
//{"source"="src","respone"="response", [{"target"="t1"},{"target"="t2"}],[{"param"=p1},{"param"=p2"}]}
//transmission
//A.
//1.view->(request)->model
//2.model->(response)->view
//B.
//
//

///////////////////////////////////////
//Normal Control.
//template<class OBJECT,
//	class REQUEST_LOGIC,
//	template<class, class> class REQUEST
//>
//class Control
//{
//public:
//	typedef OBJECT object_t;
//	typedef REQUEST_LOGIC rqt_logic_t;
//	typedef REQUEST<object_t, rqt_logic_t> request_t;
//	typedef typename object_t::response_t response_t;
//	
//	//template<class VIEW> 
//	//using rsp_filter_t = typename response_t::FilterT<VIEW>;
//	
//public:
//	Control(object_t *obj, rqt_logic_t *logic, response_t *rsp)
//		:_obj(obj)
//		,_rsp(rsp)
//	{
//		_rqt.AttachSrc(_obj);
//		_rqt.AttachLogic(logic);
//		_rsp->SetSrc(_obj);
//		_obj->Attach(rsp);
//	}
//
//	Control(object_t *obj, rqt_logic_t *logic)
//		:_obj(obj)
//	{
//		_rqt = std::unique_ptr<request_t>(new request_t());
//		_rqt.AttachSrc(_obj);
//		_rqt.AttachLogic(logic);
//	}
//	
//	Control(rqt_logic_t *logic, response_t *rsp)
//		:_rsp(rsp)
//	{
//		_rqt = std::unique_ptr<request_t>(new request_t());
//		_rqt.AttachSrc(_obj);
//		_rqt.AttachLogic(logic);
//	}
//
//	Control(rqt_logic_t *logic)
//	{
//		_rqt = std::unique_ptr<request_t>(new request_t());
//		_rqt.AttachSrc(_obj);
//		_rqt.AttachLogic(logic);
//	}
//
//	void AttachView(typename response_t::view_t *view){ _rsp->AttachView(view); }
//	
//	template<class... Ts>
//	int Execute(Ts&... ts) const
//	{
//		int r = _rqt.Execute(ts...);
//		if( r == 0 && _rsp != nullptr )
//			r = _rsp->Update();
//			
//		return r;
//	}
//
//	template<class... Ts>
//	int Execute(Ts*... ts) const
//	{
//		int r = _rqt.Execute(ts...);
//		if (r == 0 && _rsp != nullptr)
//			r = _rsp->Update();
//
//		return r;
//	}
//	
//private:
//	object_t *_obj = nullptr;
//	request_t _rqt;
//	response_t *_rsp = nullptr;
//};
//
////////////////////////////////////////////////
////Asynchronous Control.
////Inter threads.
//template<class OBJECT
//	,class REQUEST_LOGIC
//	,template<class, class> class REQUEST 
//>
//class AsynControl
//{
//public:
//	typedef OBJECT object_t;
//	typedef REQUEST_LOGIC rqt_logic_t;
//	typedef REQUEST<object_t, rqt_logic_t> request_t;
//	typedef typename object_t::response_t response_t;
//	typedef AsynControl<object_t, rqt_logic_t, REQUEST> my_t;
//	struct Work
//	{
//		Work(my_t *ctrl)
//			:ctrl(ctrl)
//		{
//		}
//		
//		template<class... Ts>
//		void operator()(Ts&... ts)
//		{
//			int r = ctrl->_rqt.Execute(ts...);
//			if( r == 0 && ctrl->_rsp != nullptr )
//				ctrl->_rsp->Update();			
//		}
//		
//		my_t *ctrl;
//	};
//	
//	friend struct Work;
//};

//////////////////////////////////////////////
//Remote Control.
//Inter-proccess.
//@PACK inter pack.
//@OBJECT model.
//@LOGIC request method.
template<class PACK, 
	class OBJECT
	,class LOGIC
>
class RControl
{

public:
	typedef PACK pack_t;
	typedef pack_t::params_pack_t 	params_pack_t;
	typedef pack_t::pack_ptr_t  	pack_ptr_t;
	typedef pack_t::pack_list_t 	pack_list_t;
	
	typedef OBJECT object_t;
	typedef LOGIC logic_t;
	typedef RRequest<object_t, logic_t> request_t;
	typedef RResponse<pack_t> response_t;
	
	
	const static std::string rqt_name(){ return logic_t::name; }
	
	RControl(object_t *obj, logic_t *logic)
		:obj(obj)
		,_rsp(obj->Name())
	{
		_rqt.AttachSrc(obj);
		_rqt.AttachLogic(logic);
	}
	
	RControl()
	{
		_rqt.AttachLogic( new logic_t(&_rsp) );
	}
	
	void AttachObj(object_t *obj)
	{
		_obj = obj
		_rsp.SetSrc(_obj);			
		_rqt.AttachSrc(_obj);
	}

	bool Execute(pack_t const& pck)
	{
		typename pack_t::params_pack_t prams = std::move(pck.Params());
		_state = _rqt.Execute( prams );
				
		return true;
	}
	
	pack_list_t Reply(const pack_t &src_pck )
	{
		pack_list_t pcks;
		pack_ptr_t pck( _rsp.Reply(src_pck) );
		if(pck!=nullptr && pck->Status())
			pcks.push_back(pck);
		else
		{
			if(_state == 0)
			{
				pcks.push_back( pack_ptr_t(
					new pack_t(src_pck.Target(), src_pck.Source(), "response", "Resqust OK")
					)
				);
			}
			else
			{
				pcks.push_back(pack_ptr_t(
					new pack_t(src_pck.Target(), src_pck.Source(), "response", "Resqust Failed")
					)
				);
			}
		}
				
		return std::move( pcks );
	}

private:
	object_t *_obj = nullptr;
	request_t  _rqt;
	response_t _rsp;
	int _state = 0;
};

NAMESP_END;

#endif /*MVC_CONTROL_H*/
