//file: mvcresponse.h

#ifndef MVC_RESPONSE_H
#define MVC_RESPONSE_H

#ifndef MVCRESPONSE_ABSTR_H
#include "mvcresponse_abstr.h"
#endif

#ifndef GLOBALDEF_H
#include "globaldef.h"
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
#include <vector>
#include <string>
#include <map>

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

template<class REQUEST_SOURCE>
class RResponse:
	public IResponse
{
public:
	typedef std::string view_t;
	typedef REQUEST_SOURCE source_t;
		
	class FilterT
	{
	public:
		typedef typename RResponse::source_t obj_t;
		typedef typename RResponse::view_t view_t;
		typedef std::map<std::string, std::string> param_pack_t;
		
		param_pack_t Filter(view_t view, obj_t *obj)
		{	
			return std::move<param_pack_t>( Transform( view, obj ) );
		}
		
		virtual param_pack_t Transform(view_t view, obj_t *obj)=0;
	};
	
public:
	RResponse(source_t *psrc, FilterT *filter)
		:_src(psrc)
		,_filter(filter)
	{
	}
	
	RResponse(FilterT *filter)
		:_src(nullptr)
		,_filter(filter)
	{
	}
	
	void SetSrc(source_t *src){ _src = src; }
	void AttachView(view_t view){ _views.push_back(view); }
	
	int Update() override
	{
		for( auto &i:_views )
		{
			 _params = _filter->Filter( i, _src);			
		}
		return 0;
	}
	
	template<class PACK>
	std::vector<PACK> Reply()
	{
		std::vector<PACK> pcks;
		for(auto &i:_views)
		{			
			pcks.push_back( std::move<PACK>( PACK(i, _src->Name(), "response", _params) ) );
		}
		
		return std::move<std::vector<PACK>& >(pcks);
	}
				
protected:
	std::shared_ptr<FilterT > _filter;
	std::vector<view_t> _views;
	typename FilterT::param_pack_t _params;
	source_t *_src;
};


NAMESP_END;

#endif /*MVC_RESPONSE_H*/
