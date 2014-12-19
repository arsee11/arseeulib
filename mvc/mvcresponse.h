//mvcresponse.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-15
//record 
//****************************

#ifndef MVC_RESPONSE_H
#define MVC_RESPONSE_H

#ifndef MVCRESPONSE_ABSTR_H
#include "mvcresponse_abstr.h"
#endif

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#include <string>
#include <memory>

#ifndef MVCVIEW_ABSTR_H
#include "mvcview_abstr.h"
#endif

#ifndef MVC_VIEW_H
#include "mvcview.h"
#endif

#include <list>

NAMESP_BEGIN

template<class REQUEST_SOURCE>
class Response:
	public IResponse
{
public:
	typedef IView view_t;
	typedef REQUEST_SOURCE source_t;
	
	template<class VIEW>
	class FilterT:
		public IFilter<source_t>
	{
	public:
		typedef VIEW view_t;
		typedef source_t obj_t;
			
		bool Filter(IView *basev, obj_t *obj) override
		{	
			view_t *view = (view_t *)basev;		
			return Transform( view, obj );
		}
		
		virtual bool Transform(view_t * view, obj_t *obj)=0;
	};
	
public:
	template<class VIEW>
	Response(source_t *psrc, FilterT<VIEW> *filter)
		:_src(psrc)
		,_filter(filter)
	{
	}
	
	template<class VIEW>
	Response(FilterT<VIEW> *filter)
		:_src(nullptr)
		,_filter(filter)
	{
	}
	
	void SetSrc(source_t *src){ _src = src; }
	void AttachView(view_t *view){ _views.push_back(view); }
	
	int Update() override
	{
		for( auto &i:_views )
		{
			if( _filter->Filter( i, _src) )	
				i->Output();
		}
		return 0;
	}
	
	
	//bool Serialize();
	//bool UnSerialize();

	
protected:
	std::shared_ptr<IFilter<source_t> > _filter;
	std::list<view_t* > _views;
	source_t *_src;
};



/////////////////////////////////////////////////
//RResponse
//Remote Response
template<class PACK>
class RResponse:
	public IResponse
{
public:
	typedef REQUEST_SOURCE source_t;
	typedef ::view_t view_t;
	typedef PACK pack_t;
	typedef pack_t::params_pack_t 	params_pack_t;
	typedef pack_t::pack_ptr_t  	pack_ptr_t;
	typedef pack_t::pack_list_t 	pack_list_t;
	
public:
	RResponse(const std::string &src)
		:_src(src)
	{
	}
	
	RResponse(const std::string &&src)
		:_src(src)
	{
	}
	
	void SetSrc(const std::string &src){ _src = src; }
	void SetSrc(const std::string &&src){ _src = src; }
	
	void AttachView(const view_t &view){ _views.push_back(view); }
	void DetachView(const view_t &view)
	{ 
		auto it = std::find(_views.begin(), _views.end(), view );
		if( it != _views.end() )
			_views.erase( it );
	}
	
	void AddParam(std::string pname, std::pvalue){ _params[panme] = pvalue; }
	
	
	template<class Session>
	int Push(const params_pack_t &params, Session &ss) override
	{
		for(auto i:_views)
		{			
			_pcks.push_back( std::move( pack_t(i, _src, "push", params) ) );
		}
		
		typename pack_t::serial_t serial;
		for(auto &i : _packs)
		{
			size_t outbuf_size=0;
			char * _outbuf = serial(ip, &outbuf_size);
			ss.PostOutput(out_buf, outbuf_size);
		}
		
		return 0;
	}
	
	pack_t *Reply(pack_t &src_pck)
	{
		if(_params.size() > 0)
			return new pack_t(src_pck.Source(), _src, "response", _params);
			
		return nullptr;
	}
				
protected:
	std::shared_ptr<FilterT > _filter;
	std::vector<view_t> _views;
	params_pack_t _params;
	pack_list_t _pcks;
	std::string _src;
};


NAMESP_END;

#endif /*MVC_RESPONSE_H*/
