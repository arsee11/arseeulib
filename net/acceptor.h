//acceptor.h

#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <string>

#include <iostream>

#ifndef FD_DEF_H
#include "fddef.h"
#endif

//session 生命周期谁管理？
template<class SESSION>
class Acceptor 
{
public:
	typedef std::shared_ptr<SESSION> session_ptr_t;

public:
	Acceptor(const char* ip, unsigned short port)
		:_ip(ip)
		,_port(port)
	{
		Create();
	}

	virtual ~Acceptor()
	{
	}

	void Create()
	{
		sockaddr_in addr;
		bzero(&addr, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(_port);
		_fd = socket(AF_INET, SOCK_STREAM, 0);
		bind(_fd, (sockaddr*)&addr, sizeof(addr));
		listen(_fd, 5);
	}

	session_ptr_t AcceptHandle(fd_t newfd, const char* ip, unsigned short port)
	{
#ifdef DEBUG
		cout<<"new session:"<<"ip="<<ip<<",port="<<port<<endl;
#endif
		return session_ptr_t(new SESSION(newfd, ip, port) );	
	}


	fd_t fd(){ return _fd; }

	bool  evt_in = false;
	bool  evt_out = false;
	bool  evt_close = false;
	bool  evt_accept = true;

private:
	fd_t _fd;
	std::string _ip;
	unsigned short _port = -1;
};

#endif /*ACCEPTOR_H*/
