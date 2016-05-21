//sever.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify	
//data	: 2014-11-26
//log	: create 
//****************************

#ifndef SERVER_H
#define SERVER_H

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef UDPSOCk_H
#include "../net/udpsock.h"
#endif

#ifndef REQUEST_CONTEXT_H
#include "request_context.h"
#endif

#ifndef DISPATCHER_H
#include "dispatcher.h"
#endif

NAMESP_BEGIN

//////////////////////////////////////////////////////
template<class Pack, class... Dispachters>
class UdpServer
{
	typedef Pack pack_t;
	typedef typename pack_t::pack_ptr_t pack_ptr_t;

	enum{ MaxBufSize=65535 };

	

public:
	UdpServer(unsigned short local_port)
	{
		Init(local_port);
	}

	~UdpServer()
	{
		Close();
	}

	template<class Loger>
	bool Run(Loger& loger)throw(std::exception)
	{
		pack_t pck;
		Read(pck);
		if(pck.status())
		{
			RequestContext context = {0, _remote_addr.ip, _remote_addr.port};
			typename pack_t::pack_list_t replies;
			try{
				replies = DispatcherHandler<Dispachters...>::Handle(context, pck);
			}
			catch(dispatcher_notfound_exp& e){
				//errorhandle
				loger.add(e.what(), __FILE__, __LINE__);
//				return false;
			}

			for (auto &i : replies)
			{
				Write(i);
			}
		}
		else
		{
			loger.add("pack read error",__FILE__, __LINE__);
		}

		return true;
	}
	
private:
	void Init(unsigned short local_port)
	{
		_udp = std::unique_ptr<net::UdpPeer>(net::UdpSock::Create(local_port));
	}
	
	void Close()
	{
		_udp->Close();
	}

	void Read(pack_t &pck)
	{
		net::UdpPeer::byte_t buf[MaxBufSize];
		size_t len = _udp->Read(buf, MaxBufSize, _remote_addr);
		cout<<"len:"<<len<<endl;
		cout<<"rev("<<*((int*)(buf+4))<<"):"<<std::string(buf+8)<<endl;
		typename pack_t::unserial_t us(65535);
		us(pck, buf, len); 
	}

	void Write(pack_ptr_t& pck)
	{
		typename pack_t::serial_t s;
		size_t len = 0;
		const char* stream = s(pck, &len);
		_udp->Write(stream, len, _remote_addr);
		cout<<"len:"<<len<<endl;
		cout<<"send:"<<std::string(stream+8)<<endl;
	}

private:
	std::unique_ptr<net::UdpPeer> _udp;
	net::AddrPair _remote_addr;

};

NAMESP_END

#endif/*SERVER_H*/
