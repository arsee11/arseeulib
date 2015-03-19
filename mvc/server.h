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

#ifndef MVC_OBJECT_H
#include "mvcobject.h"
#endif

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#include <exception>

#ifndef UDPSOCK_H
#include "../net/udpsock.h"
#endif

NAMESP_BEGIN

//////////////////////////////////////////////////////
template<class Channel, class ObjCollection, class... Dispachters>
class UdpServer
{
	typedef typename Channel::pack_t pack_t;
	typedef typename Channel::conf_t conf_t;

public:
	UdpServer(conf_t &conf)
	{
		_chn.Init(conf);
	}

	bool Run()throw(std::exception)
	{
		net::AddrPair addr;
		pack_t pck;
		_chn.Read(pck);

		std::vector<pack_t> replies;
		ArgIteration<Dispachters...>::Handle(ObjCollection::Instance(), pck, replies);

		for (auto &i : replies)
		{
			_chn.Write(i);
		}
		return true;
	}

private:
	enum{ DispatcherCount = ArgCounter<Dispachters...>::value };

private:
	Channel _chn;
};

//////////////////////////////////////////////////////
template<class Preactor, class Acceptor>
class PreactorServer
{
public:
	//typedef typename TransType<ObjectsCollection, Dispachters...>::result objs_colletion_t;
	//typedef typename chn_t::pack_t pack_t;

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
	//enum{ DispatcherCount = ArgCounter<Dispachters...>::value };

private:
	Preactor _preactor;
};

NAMESP_END

#endif/*SERVER_H*/
