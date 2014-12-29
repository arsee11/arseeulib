//addr.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify	
//data	: 2014-12-11
//log	: create 
//****************************

#ifndef ADDR_H
#define ADDR_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#include <exception>
#include <string>

#if defined(_MSC_VER)
#include <winsock2.h>
#endif

#if defined(__GNUC__)
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <string>


NAMESP_BEGIN

#if defined(__GNUC__)
typedef int SOCKET;
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#endif

class sockexcpt:public std::exception
{
public:
	sockexcpt(const char *str)
		:_str(str)
	{}

	const char* what()const throw()
	{
		return _str.c_str();
	}

private:
	std::string _str;
};

class SockConfig
{
public:
	SockConfig(unsigned short inlport, unsigned short inrport, std::string inlip, std::string inrip)
		:lport(inlport)
		, rport(inrport)
		, lip(inlip)
		, rip(inrip)
	{
	}
	
	
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

inline bool SockInit()
{
#if defined(_MSC_VER)
	WSADATA wsaData;
	int r = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (r != NO_ERROR)
	{
       		return false;
	}
#endif	
	return true;
}

inline bool SockUninit()
{
#if defined(_MSC_VER)
	return WSACleanup()==0;
#else
	return true;
#endif
}
	
NAMESP_END

#endif/*ADDR_H*/
