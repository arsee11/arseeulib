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
#include "reqest_context.h"
#endif

#ifndef DISPATCHER_H
#include "dispatcher.h"
#endif

NAMESP_BEGIN

//////////////////////////////////////////////////////
template<class Pack, class... Dispachters>
class UdpServer
{
	typedef typename Pack pack_t;

	enum{ MaxBufSize=65535 };

	

public:
	UdpServer(unsigned short local_port)
	{
		Init(port);
	}

	~UdpServer(conf_t &conf)
	{
		close();
	}

	template< class Objs, class Loger>
	bool Run(Objs& objs, Loger& loger)throw(std::exception)
	{
		pack_t pck;
		Read(pck);
		if(pck.status())
		{
			Requestconext context = {0, _remote_addr.ip, _remote_addr.port};
			typename pack_t::pack_list_t replies =
			try{
				DispatcherHandler<Dispachters...>::Handle(context, objs, pck);
			}
			catch(dispatcher_notfound_exp& e){
				//errorhandle
				loger.add(e.what(), __LINE__);
				return false;
			}

			for (auto &i : replies)
			{
				Write(i);
			}
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
		_udp->Read(buf, MaxBufSize, _remote_addr);
		typename pack_t::unserial_t unserializer_t;
		auto fuc = std::bind(unserializer_t, std::placeholders::_1);
		pck = std::move( fuc( pack_t::stream_t(buf) ) );
	}

	void Write(pack_t &pck)
	{
		typename pack_t::serial_t serializer_t;
		auto fuc = std::bind(serializer_t, pck, std::placeholders::_1);
		size_t len = 0;
		typename pack_t::stream_t stream = fuc(len);
		_udp->Write(stream.c_str(), len, _remote_addr);
	}

private:
	std::unique_ptr<net::UdpPeer> _udp;
	net::AddrPair _remote_addr;

};


//////////////////////////////////////////////////////
template<class Preactor, class Acceptor>
class PreactorServer
{
public:
	PreactorServer(size_t max_session, net::SockConfig &conf)
		:_preactor(max_session)
	{
		_preactor.RegistryAcceptor(new Acceptor(conf.lip.c_str(), conf.lport));
	}

	bool Run()throw()
	{
		try{
			_preactor.EventLoop();
		}
		catch (...)
		{
			//error handle
		}

		return true;
	}

private:
	Preactor _preactor;
};

NAMESP_END

#endif/*SERVER_H*/