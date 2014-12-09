//file:tcpsock.h


#ifndef UDPSOCK_H
#define UDPSOCK_H

#ifndef GLOBALDEF_H
#include "globaldef.h"
#endif

#include <string>
#include <exception>

#if define(MVC_VER)
#include <winsock2.h>
#endif

NAMESP_BEGIN

struct SockConfig
{
	unsigned short lport;
	unsigned short rport;
	std::string 	 lip;
	std::string 	 rip;		
};

struct AddrPair
{
	unsigned short 	port;
	std::string 	ip;
};

class RemotePeer
{
	typedef RemotePeer my_t;
public:
	RemotePeer(){}

	RemotePeer(SOCKET s, const SockConfig &conf)
		:_sock(s)
	{
		memset(&_raddr, 0, sizeof(_raddr));
		_raddr.sin_family = AF_INET;
		_raddr.sin_addr.s_addr = inet_addr(conf.rip.c_str());
		_raddr.sin_port = htons(conf.rport);

		memset(&_laddr, 0, sizeof(_laddr));
		_laddr.sin_family = AF_INET;
		_laddr.sin_addr.s_addr = inet_addr(conf.lip.c_str());
		_laddr.sin_port = htons(conf.lport);
	}
	
	int Read(char *buf, int len);
	int Write(const char *buf, int len);

	SOCKET SOCK(){ return _sock; }

	bool operator==(const my_t &rhs)
	{
		if (_sock == rhs._sock)
			return true;

		return false;
	}
	
	void Close()
	{
		closesocket(_sock);
	}

private:
	SOCKET _sock;

	std::string _ip;
	unsigned short _port;
	SOCKADDR_IN _laddr;
	SOCKADDR_IN _raddr;

};


////////////////////////////////////////////////////////////////////////
class LocalPeer
{
	typedef LocalPeer my_t;
public:
	LocalPeer(){}

	LocalPeer(SOCKET s, const std::string &ip, unsigned short port)
		:_sock(s)
		,_ip(ip)
		,_port(port)
	{
		memset(&_laddr, 0, sizeof(_laddr));
		_laddr.sin_family = AF_INET;
		_laddr.sin_addr.s_addr = inet_addr(ip.c_str());
		_laddr.sin_port = htons(port);
	}

	RemotePeer Accept(char *buf, int len);

	SOCKET SOCK(){ return _sock; }

	bool operator==(const my_t &rhs)
	{
		if (_sock == rhs._sock
		)
			return true;

		return false;
	}
	
	void Close()
	{
		closesocket(_sock);
	}

private:
	SOCKET _sock;

	std::string _ip;
	unsigned short _port;
	SOCKADDR_IN _laddr;
};


class TcpSock
{
public:
	typedef std::shared_ptr<RemotePeer> rpeer_ptr_t;	
	typedef std::shared_ptr<LocalPeer> lpeer_ptr_t;	

public:
	static bool Init();
	static bool UnInit();
	static rpeer_ptr_t CreateClient(const std::string &ip, unsigned short port) throw(std::exception);	
	static lpeer_ptr_t CreateServer(const std::string &ip, unsigned short port) throw(std::exception);	
};

NAMESP_END

#endif /*UDPSOCK_H*/
