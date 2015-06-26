//sequenceid_guard.h

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

class SequenceIDHandler
{
	const int INCREMENT = 1;
	
public:
	typedef struct ID{
		ID(const char* id)
		{
			value = str2t<size_t>(id);
		}
		
		size_t value;
		
	}id_t;
	
	void put_old(id_t& id){ _id_old = id; }
	void put_new(id_t& id){ _id_new = id; }
	
	bool operator()()
	{
		return (_id_new.value-_id_old.value == INCREMENT);
	}
	
	id_t _id_old;
	id_t _id_new;
	
};

}//net
NAMESP_END