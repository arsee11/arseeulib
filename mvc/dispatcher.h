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

NAMESP_BEGIN


template<class CONTROL>
class Dispatcher
{
public:
	typedef CONTROL control_t;
	typedef typename control_t::source_t trans_t;
	//typedef typename control_t::chn_t chn_t;
	//typedef typename chn_t::pack_t pack_t;

public:
	template<class OBJECTS_COLLECTION, class Pack>
	static void Execute(OBJECTS_COLLECTION &objs, Pack &pck, std::vector<Pack> &replies)
	{
		typedef Pack pack_t;
		//cout << pck.Action() << endl;
		if ( pck.Action() == control_t::rqt_name() )
		{
			control_t ctrl;
			//std::string tname = pck.Target();
			std::string tname = control_t::target();
			objs.GetObj(tname, ctrl);
			ctrl.Execute(pck);
			replies = ctrl.Reply(pck);
		}
	}
};


NAMESP_END

#endif /*DISPATCHER_H*/
