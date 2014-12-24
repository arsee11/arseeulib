//receiver.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-20
//record 
//****************************

#ifndef RECEIVER_H
#define RECEIVER_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#include <string>

NAMESP_BEGIN

struct Receiver
{
	int id;
	std::string ip;
	unsigned short port;
};

NAMESP_END

#endif /*RECEIVER_H*/
