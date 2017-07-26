//connection.h
//copyright	:Copyright (c) 2014 arsee.
//license	:GNU GPL V2.

#ifndef CONNECTION_H
#define CONNECTION_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

class Conntection
{
public:
	int write(void* buf, len)throw(sockexp);	

private:
	fd_t _fd;
};



}//net
NAMESP_END

#endif /*CONNECTION_H*/

