//fddef.h
//copyright	:Copyright (c) 2014 arsee.
//license	:GNU GPL V2.

#ifndef FD_DEF_H
#define FD_DEF_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

using namespace std;

NAMESP_BEGIN
namespace net
{

NAMESP_BEGIN
namespace net
{

typedef int fd_t;

enum EvtType{
	EVT_IN =0 ,
	EVT_OUT ,
	EVT_CLOSE,
	EVT_ACCEPT,
	EVT_NULL
};

}//net
NAMESP_END

#endif/*FD_DEF_H*/
