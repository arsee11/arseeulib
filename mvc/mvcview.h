//mvcview.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-17
//record 
//****************************

#ifndef MVC_VIEW_H
#define MVC_VIEW_H

#ifndef MVCVIEW_ABSTR_H
#include "mvcview_abstr.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef INVOKED_H
#include "invoked.h"
#endif

#include <string>
#include <utility>

NAMESP_BEGIN

template<int n, class Derived, class... Ts>
using MVCView = Invoked<n, Derived, Ts...>;
//{
//public:
//	typedef View<param_count> my_t;
//	
//public:
//	template<class... Us>
//	bool Execute(Us&&... params)
//	{
//		return _t.Receive( params... );
//	}
//	
//	template<class... Us>
//	bool Execute(Us&... params)
//	{
//		return _t.Receive( params... );
//	}
//	
//	const std::string name(){ return _t.name(); }
//		
//	T& ref(){ return (_t); }
//	
//private:
//	T _t;
//};



NAMESP_END;

#endif /*MVC_VIEW_H*/
