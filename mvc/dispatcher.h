//file: dispatcher.h

#ifndef DISPATCHER_H
#define DISPATCHER_H

#ifndef GLOBALDEF_H
#include "globaldef.h"
#endif

#ifndef JPACK_H
#include "jpack.h"
#endif

NAMESP_BEGIN

///////////////////////////////////////////////////////////
//template<class CONTROL>
//class Dispatcher
//{
//public:
//	typedef CONTROL control_t;
//	typedef typename control_t::source_t trans_t;
//	//typedef typename control_t::chn_t chn_t;
//	//typedef typename chn_t::pack_t pack_t;
//	
//public:
//	template<class OBJECTS_COLLECTION, class CHANNEL>
//	static void Execute(OBJECTS_COLLECTION &objs, CHANNEL &chn)
//	{
//		typedef typename CHANNEL::pack_t pack_t;
//		pack_t pck = chn.Head();
//		//cout << pck.Action() << endl;
//		if( pck.Action() == control_t::rqt_name )
//		{
//			control_t ctrl;
//			objs.GetObj(pck.Target(), ctrl);
//			chn.Input( std::bind(&control_t::Execute<pack_t>, std::ref(ctrl), std::placeholders::_1), pck );
//			chn.Output( std::bind(&control_t::Reply<pack_t>, std::ref(ctrl), std::placeholders::_1), pck );
//		}
//	}
//};

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
		if (pck.Action() == control_t::rqt_name)
		{
			control_t ctrl;
			objs.GetObj(pck.Target(), ctrl);
			ctrl.Execute(pck);
			replies = ctrl.Reply(pck);
		}
	}
};
NAMESP_END

#endif /*DISPATCHER_H*/