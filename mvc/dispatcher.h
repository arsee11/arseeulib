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

#include <iostream>


NAMESP_BEGIN


template<class CONTROL>
class Dispatcher
{
public:
	typedef CONTROL control_t;
	
public:
	template<class Receiver, class OBJECTS_COLLECTION, class Pack>
	static void Execute(Receiver& rev, OBJECTS_COLLECTION &objs, Pack &pck, typename Pack::pack_list_t &replies)
	{
		typedef Pack pack_t;
		//std::cout << pck.Action() << std::endl;
		if ( pck.action() == control_t::rqt_name() )
		{
			std::string tname = control_t::target();
			control_t ctrl(pck.source(), &rev);
			objs.GetObj(tname, ctrl, pck);
			ctrl.Reply(replies);
		}
	}
};


NAMESP_END

#endif /*DISPATCHER_H*/
