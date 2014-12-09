//file: udpsock.cpp

#ifndef UDPSOCK_H
#include "udpsock.h"
#endif

#pragma comment(lib, "Ws2_32.lib")

NAMESP_BEGIN

long SockConfig::sid=0;

bool UdpSock::Init()
{
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (r != NO_ERROR)
	{
        return false;
    }
	
	return true;
}

bool UdpSock::UnInit()
{
	return WSACleanup();
}

UdpPeer* UdpSock::Create(const std::string &ip, unsigned short port) throw(std::exception)
{
	SOCKET sock;
	if ((sock=socket(AF_INET, SOCK_DGRAM,0))== INVALID_SOCKET)
	{ 
		throw std::exception("invalid_socket");
	} 
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr( ip.c_str() );
	if (bind(sock, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		throw std::exception("bind error");
	}
	
	return new UdpPeer(sock, ip, port);
}
UdpPeer* UdpSock::Create(unsigned short port) throw(std::exception)
{
	SOCKET sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		throw std::exception("invalid_socket");
	}
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		throw std::exception("bind error");
	}

	return new UdpPeer(sock,port);
}

UdpPeer* UdpSock::Create() throw(std::exception)
{
	SOCKET sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		throw std::exception("invalid_socket");
	}
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(0);
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		throw std::exception("bind error");
	}

	return new UdpPeer(sock);
}



int UdpPeer::Read(byte_t *buf, int len, AddrPair &addr)
{
	SOCKADDR_IN inaddr;
	int addrlen = sizeof(inaddr);
	memset(&inaddr, 0, sizeof(inaddr));
	int rlen=-1;
	if( ( rlen=recvfrom(_sock, buf, len, 0, (LPSOCKADDR)&inaddr, &addrlen) )>=0 )
	{
		addr.ip = inet_ntoa(inaddr.sin_addr);
		addr.port = ntohs(inaddr.sin_port);
	}
	
	return rlen;
}

int UdpPeer::Write(const byte_t *buf, int len, const AddrPair &addr)
{
	sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(addr.ip.c_str());
	saddr.sin_port = htons(addr.port);
	return sendto( _sock, buf, len, 0, (LPSOCKADDR)&saddr, sizeof(saddr) );
}

NAMESP_END