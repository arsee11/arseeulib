//replay_guard.h
//copyright	:Copyright (c) 2015 arsee.
//license	:GNU GPL V2.

#ifndef REPLAY_GUARD_H
#define REPLAY_GUARD_H

#ifndef NAMESPDEF_H
#include "../../namespdef.h"
#endif


NAMESP_BEGIN
namespace net
{

template<class Handler, class Session>
class ReplayGuard
{
	typedef ReplayGuard<Handler, Session> my_t;
	typedef typename Handler::id_t id_t;
	typedef std::map<Session, id_t> id_map_t;
	
	ReplayGuard(){}
	ReplayGuard(my_t&);
	my_t& operator=(my_t&);
	
public:	
	static my_t& instance()
	{
		static my_t myselft;
		return myselft;
	}
	
	bool Ask(const char* session, const char* id)
	{
		Session s(session);
		Handler h;
		h.put_old(_id_map[s]);
		id_t new_one(id);
		h.put_new(new_one);
		if( !h() )
			return false;
		
		_id_map[s] = new_one;
		return true;
	}
	
private:
	id_map_t _id_map;
};

template<class Session>
using SequenceIDGuard=ReplayGuard<SequenceIDHolder, SequenceIDHandler, Session>;

}//net
NAMESP_END