//mvc_request_context.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-20
//record 
//****************************

#ifndef MVC_REQUEST_CONTEXT_H
#define MVC_REQUEST_CONTEXT_H

#ifndef remote_id_H
#define RECEIVER_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#include <string>

#ifndef STRINGEX_H
#include "../stringex.h"
#endif

NAMESP_BEGIN

struct RequestContext
{
	int id;
	std::string ip;
	unsigned short port;

	string id_str()const{ return ip+":"+t2str(port) };
};

NAMESP_END

#endif /*MVC_REQUEST_CONTEXT_H*/
