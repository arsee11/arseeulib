//session.h

#ifndef SESSIOIN_H
#define SESSIOIN_H

#include <iostream>
#include <strings.h>
#include <string.h>
#include <memory>
#include <string>

#ifndef FD_DEF_H
#include "fddef.h"
#endif




template<size_t insize>
class Session
{
public:
	Session(fd_t fd, std::string remoteip, unsigned short remote_port)
		:_remoteip(remoteip)
		,_remote_port(remote_port)
		,_fd(fd)
	{
	}

	virtual ~Session()
	{
#ifdef DEBUG
		cout<<"~Session"<<endl;
#endif
	}

	virtual void InputHandle(size_t len)
	{

#ifdef DEBUG
		cout << "read("<<len<<"):"<< _inbuf <<endl;
#endif
	}

	virtual void OutputHandle()
	{
#ifdef DEBUG
		cout<<"out handling..."<<endl;
		char *outbuf = new char[6];
		strcpy(outbuf, "hello, ");
		_outbuf = outbuf;
		_outbuf_size = 6;
#endif
	}

	virtual void CloseHandle()
	{
#ifdef DEBUG
		cout<<_remoteip<<":"<<_remote_port<<"disconnected"<<endl;
#endif
	}

	fd_t fd(){return _fd;}
	char *inbuf(){ return _inbuf; }
	size_t in_bufsize(){ return insize; }

	const char *outbuf(){ return _outbuf; }
	size_t out_bufsize(){ return _outbuf_size; }

protected:
	std::string _remoteip;
	unsigned short _remote_port = -1;
	fd_t _fd = -1;
	char _inbuf[insize] = {0};
	//size_t _inbuf_size = 1024;
	const char *_outbuf = nullptr;
	size_t _outbuf_size = 0;
};

#endif /*SESSIOIN_H*/
