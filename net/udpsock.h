//file: udpsock.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify	
//data	: 2014-12-11
//log	: modify 
//****************************

#ifndef UDPSOCK_H
#define UDPSOCK_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef ADDR_H
#include "addr.h"
#endif

#include <functional>

NAMESP_BEGIN

namespace net
{
class UdpPeer
{
	typedef UdpPeer my_t;
	
public:
	typedef char byte_t;
	typedef AddrPair addr_t;

	const static int MAX_LEN = 65535;

private:
	UdpPeer(SOCKET s)
		:_sock(s)
		, _ip("")
		, _port(0)
	{
	}

	UdpPeer(SOCKET s, const std::string &lip, unsigned short lport)
		:_sock(s)
		,_ip(lip)
		,_port(lport)
	{

	}
	
	UdpPeer(SOCKET s, unsigned short lport)
		:_sock(s)
		,_ip("")
		,_port(lport)
	{

	}

	friend class UdpSock;
	
public:
	int Read(byte_t *buf, int len, AddrPair &addr);
	int Read(byte_t *buf, int len);
	int Write(const byte_t *buf, int len, const AddrPair &addr);
		
	SOCKET sock(){ return _sock; }

	void Close()
	{
#if defined(_MSC_VER)
		closesocket(_sock);
#else
		close(_sock);
#endif
		_sock = INVALID_SOCKET;
	}

private:
	SOCKET _sock;
	std::string _ip;
	unsigned short _port;

};

class UdpSock
{
public:
	static UdpPeer* Create(const std::string &lip, unsigned short lport) throw(sockexcpt);	
	static UdpPeer* Create(unsigned short lport) throw(sockexcpt);	
	static UdpPeer* Create() throw(sockexcpt);	
};

}//net
NAMESP_END

#endif /*UDPSOCK_H*/
