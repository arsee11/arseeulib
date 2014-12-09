//fddef.h

#ifndef FD_DEF_H
#define FD_DEF_H

typedef int fd_t;

enum EvtType{
	EVT_IN =0 ,
	EVT_OUT ,
	EVT_CLOSE,
	EVT_ACCEPT,
	EVT_NULL
};

#endif/*FD_DEF_H*/
