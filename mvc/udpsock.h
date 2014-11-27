//file: udpsock.h

#ifndef UDPSOCK_H
#define UDPSOCK_H

#ifndef GLOBALDEF_H
#include "globaldef.h"
#endif

#include <string>
#include <exception>
#include <functional>

#if defined(_MSC_VER)
#include <winsock2.h>
#endif

#if defined(__GNUC__)
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

NAMESP_BEGIN

#if defined(__GNUC__)
typedef int SOCKET;
#endif

class SockConfig
{
public:
	SockConfig(unsigned short inlport, unsigned short inrport, std::string inlip, std::string inrip)
		:lport(inlport)
		, rport(inrport)
		, lip(inlip)
		, rip(inrip)
	{
		sid++;
		id = sid;
	}
	

	static long sid;
	unsigned long Hash()const
	{
		//std::hash<unsigned short> h1;
		//std::hash<std::string> h2;
		//std::hash<unsigned long> h3;
		//
		//return h3( h1(lport)+h1(rport)+h2(lip)+h2(rip) );
		return id;
	}
	
	unsigned short lport;
	unsigned short rport;
	std::string 	 lip;
	std::string 	 rip;	

private:
	long id;
	SockConfig(const SockConfig &other);
	SockConfig& operator=(const SockConfig& rhs);
};

struct AddrPair
{
	unsigned short 	port;
	std::string 	ip;
};



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
	int Write(const byte_t *buf, int len, const AddrPair &addr);
		
	SOCKET Socket(){ return _sock; }

	void Close()
	{
#if defined(_MSC_VER)
		closesocket(_sock);
#endif
		close(_sock);
	}

private:
	SOCKET _sock;

	std::string _ip;
	unsigned short _port;

};

class UdpSock
{
public:
	static bool Init();
	static bool UnInit();
	static UdpPeer* Create(const std::string &lip, unsigned short lport) throw(std::exception);	
	static UdpPeer* Create(unsigned short lport) throw(std::exception);	
	static UdpPeer* Create() throw(std::exception);	
};


NAMESP_END

#endif /*UDPSOCK_H*/
