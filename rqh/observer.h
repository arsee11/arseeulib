//observer.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2015-01-05
//record 
//****************************

#ifndef OBSERVER_H
#define OBSERVER_H

#include <string>
#include <exception>


#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif


#ifndef JPACK_H
#include "jpack.h"
#endif

#ifndef UDPSOCK_H 
#include "../net/udpsock.h"
#endif

#ifndef TCPSOCK_H 
#include "../net/tcpsock.h"
#endif

#ifndef MVCLRESPONSE_H
#include "mvclresponse.h"
#endif

#include <thread>

NAMESP_BEGIN


template<class Receiver, class Pack>
class ObserverTmp
{
	typedef Pack pack_t;
	typedef LResponse<Pack> response_t;
	typedef std::shared_ptr< LResponse<Pack> > rsp_ptr_t;

public:
	ObserverTmp()=default;
	virtual ~ObserverTmp()
	{ 
		Close();
		_listen_status = false; 
		if(_listener_thread.joinable()) 
			_listener_thread.join(); 
	}		

	void Register(response_t* rsp)
	{
		_rsps[rsp->name()] = rsp_ptr_t(rsp);
	}

	void UnRegister(response_t* rsp)
	{
		_rsps.erase( rsp->name() );
	}

	void Listen()
	{
		auto fuc = [this](){		
			pack_t::unserial_t us(1024);
			while (_listen_status)
			{
				//error for 3 times
				for (int i = 0; i<3; i++)
				{
					char rbuf[1024] = { 0 };
					int len =  _rev->Read(rbuf, 1024);
					if (len > 0)
					{
						cout << "rbuf:" << rbuf + pack_t::HeadField + pack_t::LenField << endl;
						Pack::pack_list_t pcks;
						if( LResponse<Pack>::Parse(rbuf, len, pcks) )
						{
							for (auto& i : pcks)
							{
								if (_rsps.find(i->target()) != _rsps.end())
									_rsps[i->target()]->Update( *(i.get()) );
							}

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
	std::map<std::string, rsp_ptr_t > _rsps;
};


////////////////////////////////////////////////////////////////////////////////
template<class Pack>
class UdpObserver:
	public ObserverTmp<net::UdpPeer*, Pack>
{
	typedef ObserverTmp<net::UdpPeer*, Pack> base_t;
	
public:
	UdpObserver() = default;
	~UdpObserver(){ Close(); if (_rev != nullptr) delete _rev; }
	

	void Open(unsigned short lport)throw(net::sockexcpt)
	{
		base_t::_rev = net::UdpSock::Create(lport);
	}
};


typedef UdpObserver<Jpack> JUdpObserver;

////////////////////////////////////////////////////////////////////////////////
template<class Pack>
class TcpObserver:
	public ObserverTmp<net::TcpSock::rpeer_ptr_t, Pack>
{
	typedef ObserverTmp<net::TcpSock::rpeer_ptr_t, Pack> base_t;
	
public:
	TcpObserver()=default;

	void Open(unsigned short lport)throw(net::sockexcpt)
	{
		base_t::_rev = net::TcpSock::CreateServer(lport);
	}
	
	void Open(net::TcpSock::rpeer_ptr_t& conn)throw(net::sockexcpt)
	{
		base_t::_rev = conn;
	}

	void Open(net::TcpSock::rpeer_ptr_t&& conn)throw(net::sockexcpt)
	{
		base_t::_rev = conn;
	}
};


typedef TcpObserver<Jpack> JTcpObserver;

NAMESP_END

#endif/*OBSERVER_H*/
