//tcpsock.cpp
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify	
//data	: 2014-12-11
//log	: modify 
//****************************

#ifndef TCPSOCK_H
#include "tcpsock.h"
#endif

#if defined(__GNUC__)
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#endif

#if defined(_MSC_VER)
typedef int socklen_t;
#endif

NAMESP_BEGIN

TcpSock::rpeer_ptr_t TcpSock::CreateClient(unsigned short lport, const std::string &rip, unsigned short rport) throw(sockexcpt)
{
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s ==INVALID_SOCKET)
		return nullptr;
	
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(lport);
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(s, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		throw sockexcpt("bind");
	}
	
	sockaddr_in raddr;
	memset(&raddr, 0, sizeof(raddr));
	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(rport);
	raddr.sin_addr.s_addr = inet_addr(rip.c_str());
	if (connect(s, (sockaddr *)&raddr, sizeof(raddr)) == SOCKET_ERROR)
	{
		throw sockexcpt("connect");
	}
		
	return  rpeer_ptr_t(new RemotePeer(s, SockConfig(0, rport, "", rip)));
}

TcpSock::rpeer_ptr_t TcpSock::CreateClient(unsigned short lport, std::string &&rip, unsigned short rport) throw(sockexcpt)
{
	return  CreateClient(lport, rip, rport);
}

TcpSock::rpeer_ptr_t TcpSock::CreateClient(std::string &&rip, unsigned short rport) throw(sockexcpt)
{
	return CreateClient(0, rip, rport);
}

TcpSock::rpeer_ptr_t TcpSock::CreateClient(const std::string &rip, unsigned short rport) throw(sockexcpt)
{
	return CreateClient(0, rip, rport);
}

TcpSock::lpeer_ptr_t TcpSock::CreateServer(const std::string &ip, unsigned short port) throw(sockexcpt)
{
	int s = socket(AF_INET, SOCK_STREAM, 0);
	return nullptr;
}


int RemotePeer::Read(char *buf, int len)
{
	return recv(_sock, buf, len, 0);
}

int RemotePeer::Write(const char *buf, int len)
{
	return send(_sock, buf, len, 0);
}

NAMESP_END
