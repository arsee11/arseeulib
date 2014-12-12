//file: mvccontrol.h

#ifndef MVC_CONTROL_H
#define MVC_CONTROL_H

#ifndef MVC_OBJECT_H
#include "mvcobject.h"
#endif

#ifndef GLOBALDEF_H
#include "globaldef.h"
#endif

#ifndef GLOBALDEF_H
#include "../namespdef.h"
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
template<class SOURCE,
	class REQUEST_LOGIC,
	template<class, class> class REQUEST
>
class Control
{
public:
	typedef SOURCE source_t;
	typedef REQUEST_LOGIC rqt_logic_t;
	typedef REQUEST<source_t, rqt_logic_t> request_t;
	typedef typename source_t::response_t response_t;
	
	//template<class VIEW> 
	//using rsp_filter_t = typename response_t::FilterT<VIEW>;
	
public:
	Control(source_t *src, rqt_logic_t *logic, response_t *rsp)
		:_src(src)
		,_rsp(rsp)
	{
		_rqt = std::unique_ptr<request_t>(new request_t());
		_rqt->AttachSrc(_src);
		_rqt->AttachLogic(logic);
		_rsp->SetSrc(_src);
		_src->Attach(rsp);
	}

	Control(source_t *src, rqt_logic_t *logic)
		:_src(src)
	{
		_rqt = std::unique_ptr<request_t>(new request_t());
		_rqt->AttachSrc(_src);
		_rqt->AttachLogic(logic);
	}
	
	Control(rqt_logic_t *logic, response_t *rsp)
		:_rsp(rsp)
	{
		_rqt = std::unique_ptr<request_t>(new request_t());
		_rqt->AttachSrc(_src);
		_rqt->AttachLogic(logic);
	}

	Control(rqt_logic_t *logic)
	{
		_rqt = std::unique_ptr<request_t>(new request_t());
		_rqt->AttachSrc(_src);
		_rqt->AttachLogic(logic);
	}

	void AttachView(typename response_t::view_t *view){ _rsp->AttachView(view); }
	
	template<class... Ts>
	int Execute(Ts&... ts) const
	{
		int r = _rqt->Execute(ts...);
		if( r == 0 && _rsp != nullptr )
			r = _rsp->Update();
			
		return r;
	}

	template<class... Ts>
	int Execute(Ts*... ts) const
	{
		int r = _rqt->Execute(ts...);
		if (r == 0 && _rsp != nullptr)
			r = _rsp->Update();

		return r;
	}
	
private:
	source_t *_src = nullptr;
	std::unique_ptr<request_t> _rqt;
	response_t *_rsp = nullptr;
};

//////////////////////////////////////////////
//Asynchronous Control.
//Inter threads.
template<class SOURCE
	,class REQUEST_LOGIC
	,template<class, class> class REQUEST 
>
class AsynControl
{
public:
	typedef SOURCE source_t;
	typedef REQUEST_LOGIC rqt_logic_t;
	typedef REQUEST<source_t, rqt_logic_t> request_t;
	typedef typename source_t::response_t response_t;
	typedef AsynControl<source_t, rqt_logic_t, REQUEST> my_t;
	struct Work
	{
		Work(my_t *ctrl)
			:ctrl(ctrl)
		{
		}
		
		template<class... Ts>
		void operator()(Ts&... ts)
		{
			int r = ctrl->_rqt->Execute(ts...);
			if( r == 0 && ctrl->_rsp != nullptr )
				ctrl->_rsp->Update();			
		}
		
		my_t *ctrl;
	};
	
	friend struct Work;
};

//////////////////////////////////////////////
//Remote Control.
//Inter-proccess.
template<class SOURCE
	,class REQUEST_LOGIC
	,template<class, class> class REQUEST 
>
class RControl
{

public:
	typedef SOURCE source_t;
	typedef REQUEST_LOGIC rqt_logic_t;
	typedef REQUEST<source_t, rqt_logic_t> request_t;
	typedef typename source_t::response_t response_t;
	
	const static std::string rqt_name(){ return rqt_logic_t::name; }
	
	RControl(source_t *src, rqt_logic_t *logic, response_t *rsp)
		:_src(src)
		,_rsp(rsp)
	{
		_rqt = std::shared_ptr<request_t>(new request_t);
		_rqt->AttachSrc(_src);
		_rqt->AttachLogic(logic);
		_rsp->SetSrc(_src);
		_src->Attach(rsp);
	}
	
	RControl()
	{
		_rqt = std::shared_ptr<request_t>(new request_t);
		_rqt->AttachSrc(_src);
		_rqt->AttachLogic( new rqt_logic_t );
		//_rsp->SetSrc(_src);
		//_src->Attach(_rsp);
	}
	
	void AttachSrc(source_t *src)
	{
		_src = src;
		if(_rsp != nullptr)
			_rsp->SetSrc(_src);
			
		_rqt->AttachSrc(_src);
	}

	template<class PACK>
	bool Execute(PACK const& pck)
	{
		typename PACK::params_pack_t prams = std::move(pck.Params());
		_state = _rqt->Execute( prams );
				
		return true;
	}
	
	template<class PACK>
	std::vector<PACK> Reply(const PACK &src_pck )
	{
		typedef PACK pack_t;
		std::vector<pack_t> pcks;
		if(_state == 0)
		{
			if( _rsp != nullptr )
			{
				_rsp->Update();
				std::vector<pack_t> pcks= _rsp->Reply<pack_t>();
				for (auto &i:pcks)
					pcks.push_back(i);
			}
			else
			{
				pcks.push_back(pack_t(src_pck.Target(), src_pck.Source(), "response", "Resqust OK"));
			}
		}
		else
		{
			pcks.push_back(pack_t(src_pck.Target(), src_pck.Source(), "response", "Resqust Failed"));
		}
		
		return std::move( pcks );
	}

private:
	source_t *_src = nullptr;
	std::shared_ptr<request_t> _rqt;
	response_t *_rsp = nullptr;
	int _state = 0;
};


//////////////////////////////////////////////////////
//template<class CHANNEL_STORE, class...Dispachters>
//class ControlServer
//{
//	//typedef std::tuple<Dispachters...> dispth_map_t;
//	typedef CHANNEL_STORE chn_store_t;
//	
//public:
//	typedef typename chn_store_t::chn_t chn_t;
//	typedef typename chn_store_t::chn_ptr_t chn_ptr_t;
//	typedef typename TransType<ObjectsCollection, Dispachters...>::result objs_colletion_t;
//	//typedef typename chn_t::pack_t pack_t;
//
//public:
//	ControlServer(){}
//	
//	bool Run(typename chn_t::config_t &conf)throw(...)
//	{
//		_chn_store.Store(conf);
//		chn_ptr_t chn(nullptr);
//		while (_chn_store.Opening(chn))
//		{
//			ArgIteration<Dispachters...>::Handle(objs_colletion_t::Instance(), *(chn.get()));
//			chn->Pop();
//		}
//		
//		return true;
//	}
//		
//private:
//	enum{ DispatcherCount = ArgCounter<Dispachters...>::value };
//	
//private:
//	chn_store_t _chn_store;
//};

NAMESP_END;

#endif /*MVC_CONTROL_H*/
