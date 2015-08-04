//view.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-17
//record 
//****************************

#ifndef VIEW_H
#define VIEW_H

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
using View = Invoked<n, Derived, Ts...>;

NAMESP_END;

#endif /*VIEW_H*/
