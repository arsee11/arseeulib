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

#ifndef GLOBALDEF_H
#include "globaldef.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#include <string>
#include <utility>

NAMESP_BEGIN

template<class T>
class View:
	public IView
{
public:
	typedef View<T> my_t;
	
public:
	View()
	{
		_t = std::move<T>( T() );
	}
	
	template<class... Us>
	View(Us*... us)
	{
		_t =  T(us...);
	}
	
	template<class... Us>
	View(Us&... us)
	{
		_t = T(us...);
	}

	template<class... Us>
	View(Us&&... us)
	{
		_t = T(us...);
	}
	
	template<class... Us>
	bool Receive(Us&... params)
	{
		return _t.Receive( params... );
	}
	
	//template<class... Us>
	//void Receive(Us&&... params)
	//{
	//	_t.Receive( params );
	//}
	
	void Output() override
	{
		_t.Output();
	}
	
	T& ref(){ return (_t); }
	
private:
	T _t;
};


typedef std::string view_t;

NAMESP_END;

#endif /*MVC_VIEW_H*/
