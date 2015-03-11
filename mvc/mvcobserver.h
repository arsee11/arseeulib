//mvcobserver.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2015-01-05
//record 
//****************************

#ifndef MVC_OBSERVER_H
#define MVC_OBSERVER_H

#include <string>
#include <exception>


#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif


#ifndef JPACK_H
#include "jpack.h"
#endif

#ifndef TCP_SOCK_H 
#include "../net/udpsock.h"
#endif

#ifndef TCP_SOCK_H 
#include "../net/tcpsock.h"
#endif

#include <thread>

NAMESP_BEGIN


template<class Receiver, class Pack>
class ObserverTmp
{
	typedef Pack pack_t;

public:
	ObserverTmp()=default;
	virtual ~ObserverTmp()
	{ 
		Close();
		_listen_status = false; 
		if(_listener_thread.joinable()) 
			_listener_thread.join(); 
	}		

	template<class View>
	void AddListener(View &v)
	{
	}

	template<class View>
	void Listen(View *v)
	{
		auto fuc = [this, v](){		
			pack_t::unserial_t us(1024);
			while (_listen_status)
			{
				for (int i = 0; i<3; i++)
				{
					char rbuf[1024] = { 0 };
					int len =  _rev->Read(rbuf, 1024);
					if (len > 0)
					{
						LResponse<View, Pack>* rsp = new LResponse<View, Pack>(v);
						if( rsp->Parse(rbuf, len) )
						{
							rsp->Updeate(); 
							break;
						}
					}
					else
					{
						_listen_status = false;
						break;
					}
				}

			}
		};

		_listener_thread = std::thread(fuc);
	}

	void Close()
	{ 
		if(_rev != nullptr)
		{
			_rev->Close();
		}
	}
	
protected:	
	Receiver _rev = nullptr;
	std::thread _listener_thread;
	bool _listen_status = true;
};


////////////////////////////////////////////////////////////////////////////////
template<class Pack>
class UdpObserver:
	public ObserverTmp<UdpPeer*, Pack>
{
	typedef ObserverTmp<UdpPeer*, Pack> base_t;
	
public:
	UdpObserver() = default;
	~UdpObserver(){ Close(); if (_rev != nullptr) delete _rev; }
	

	void Open(unsigned short lport)throw(sockexcpt)
	{
		base_t::_rev = UdpSock::Create(lport);
	}
};


typedef UdpObserver<Jpack> JUdpObserver;

////////////////////////////////////////////////////////////////////////////////
template<class Pack>
class TcpObserver:
	public ObserverTmp<TcpSock::lpeer_ptr_t, Pack>
{
	typedef ObserverTmp<TcpSock::lpeer_ptr_t, Pack> base_t;
	
public:
	TcpObserver()=default;

	void Open(unsigned short lport)throw(sockexcpt)
	{
		base_t::_rev = TcpSock::CreateServer(lport);
	}
	
	template<class View>
	void Listen(View *v)
	{	
	}
};


typedef TcpObserver<Jpack> JTcpObserver;

NAMESP_END

#endif/*MVC_OBSERVER_H*/
