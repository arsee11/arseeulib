//file:tcpsock.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify	
//data	: 2014-12-11
//log	: modify 
//****************************

#ifndef TCPSOCK_H
#define TCPSOCK_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef ADDR_H
#include "addr.h"
#endif

#include <memory>

NAMESP_BEGIN

class RemotePeer
{
	typedef RemotePeer my_t;
public:
	RemotePeer()=delete;

	RemotePeer(SOCKET s, const SockConfig &conf)
		:_sock(s)
		,_conf(conf)
	{}
	
	RemotePeer(SOCKET s, const SockConfig &&conf)
		:_sock(s)
		,_conf(conf)
	{}
	
	~RemotePeer()
	{
		Close();
	}
	
	template<class Pack>
	int Write(Pack &pck)
	{
		Pack::serial_t ss;
		size_t len;
		const char *buf = ss(pck, &len);
		return Write(buf, len);
	}
	
	int Read(char *buf, int len);
	int Write(const char *buf, int len);

	SOCKET sock(){ return _sock; }

	bool operator==(const my_t &rhs)
	{
		if (_sock == rhs._sock)
			return true;

		return false;
	}
	
	void Close()
	{
		if(_sock != INVALID_SOCKET)
		{
#if defined(_MSC_VER)
			closesocket(_sock);
#else
			close(_sock);
#endif
			_sock = INVALID_SOCKET;
		}
	}

private:
	SOCKET _sock;
	SockConfig _conf;
};


////////////////////////////////////////////////////////////////////////
class LocalPeer
{
	typedef LocalPeer my_t;
public:
	LocalPeer()=delete;

	LocalPeer(SOCKET s, const std::string &ip, unsigned short port)
		:_sock(s)
		,_ip(ip)
		,_port(port)
	{
	}
	
	LocalPeer(SOCKET s, const std::string &&ip, unsigned short port)
		:_sock(s)
		,_ip(ip)
		,_port(port)
	{
	}

	//new RemotePeer, 
	RemotePeer* Accept(char *buf, int len);

	SOCKET sock(){ return _sock; }

	bool operator==(const my_t &rhs)
	{
		if (_sock == rhs._sock
		)
			return true;

		return false;
	}
	
	void Close()
	{
		if(_sock != INVALID_SOCKET)
		{
#if defined(_MSC_VER)
			closesocket(_sock);
#else
			close(_sock);
#endif
			_sock = INVALID_SOCKET;
		}
	}

private:
	SOCKET _sock;
	std::string _ip;
	unsigned short _port;
};


class TcpSock
{
public:
	typedef std::shared_ptr<RemotePeer> rpeer_ptr_t;	
	typedef std::shared_ptr<LocalPeer> lpeer_ptr_t;	

public:
	static bool Init();
	static bool UnInit();
	static rpeer_ptr_t CreateClient(unsigned short lport, const std::string &rip, unsigned short rport) throw(sockexcpt);
	static rpeer_ptr_t CreateClient(unsigned short lport, std::string &&rip, unsigned short rport) throw(sockexcpt);
	static rpeer_ptr_t CreateClient(std::string &&rip, unsigned short rport) throw(sockexcpt);
	static rpeer_ptr_t CreateClient(const std::string &rip, unsigned short rport) throw(sockexcpt);		
	static lpeer_ptr_t CreateServer(const std::string &ip, unsigned short port) throw(sockexcpt);	
};

NAMESP_END

#endif /*TCPSOCK_H*/
