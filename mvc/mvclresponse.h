//mvcresponseto.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-26
//record 
//****************************

#ifndef MVCLRESPONSE_H
#define MVCLRESPONSE_H

#ifndef MVCRESPONSE_H
#include "mvcresponse.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef INVOKER_H
#include "invoker.h"
#endif

NAMESP_BEGIN

template<class View, class Pack>
class LResponse
	:public Response<View, Pack>
{
public:
	LResponse(view_t *view)
		:Response("local")
		,_view(view)
	{
	}
	
	bool Parse(const char* buf, size_t len)
	{
		typename pack_t::unserial_t us(1024);
		if( us(_pck, buf, len) > 0 )
			return _pck.status();
		
		return false;
	} 
  	
	virtual void Update()
	{
		if(_view != nullptr)
		{
			if( _view->name() == _pck.target() )
				Invoker<view_t::PC>::Invoke(_pck.params(), _view);
		}
	}
	
protected:
	view_t *_view=nullptr;
	pack_t _pck;
};

NAMESP_END

#endif/*MVCLRESPONSE_H*/