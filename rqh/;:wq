//sever.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify	
//data	: 2014-11-26
//log	: create 
//****************************

#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef REQUEST_CONTEXT_H
#include "request_context.h"
#endif

#ifndef DISPATCHER_H
#include "dispatcher.h"
#endif

#ifndef REACTOR_H
#include "../net/preactor.h"
#endif

#ifndef ACCEPTOR_H
#include "../net/acceptor.h"
#endif

#ifndef FD_H
#include "../net/fd.h"
#endif

#ifndef SELECTOR_H	
#include "../net/selector.h"
#endif

#ifndef SESSION_H
#include "../net/session.h"
#endif

#include <assert.h>

NAMESP_BEGIN

enum{ MAX_BUF_LEN=1500};


typedef net::Preactor<net::FdHolder, net::Epoll> tcp_preactor_t;

template<class Pack, class Logger, class... Dispatchers>
class MyConnection :
	public net::Session<MAX_BUF_LEN, tcp_preactor_t>
{
	typedef Pack pack_t;
	typedef net::Session<MAX_BUF_LEN, tcp_preactor_t> base_t;
	
public:
	MyConnection(net::fd_t fd, const char *ip, unsigned short port)
		:base_t(fd, ip, port)
	{
	}

	void InputHandle(char* buf, size_t len)
	{
#ifdef DEBUG
		cout<<"recv("<<len<<"):"<<buf+8<<endl;
#endif
		typename pack_t::pack_list_t rpcks;
		_userial(rpcks, buf, len);
		if(rpcks.size() > 0 )
		{
			for( auto& pck : rpcks)
			{
				RequestContext context = {this->_fd, this->_remoteip, this->_remote_port};
				typename pack_t::pack_list_t pcks;
				try{
					pcks = DispatcherHandler<Dispatchers...>::Handle(context, *(pck.get()));
				}
				catch(dispatcher_notfound_exp& e){
					_log.add(e.what(), __FILE__, __LINE__);
				}

				for(auto &i : pcks)
					_replies.push_back(i);
			}
			
		}		
		else
		{
			_log.add("pack error", __FILE__, __LINE__);
		}
	}

	void OutputHandle(BufferWrapper& buf)
	{
#ifdef DEBUG
		_log.add("OutputHandle()",__FILE__, __LINE__);
#endif
		size_t totals = 0;
		for (auto &ip : _replies)
		{
			size_t bufsize=0;
			typename pack_t::serial_t serial;
			size_t stream_len;
			const char* stream = serial(ip, &stream_len);
			buf.put(stream, stream_len);
			totals+= stream_len;
		}
		
		assert(totals < buf.maxsize());
		_replies.clear();
	}
	
private:
	typename pack_t::pack_list_t _replies;
	typename pack_t::unserial_t _userial = typename pack_t::unserial_t(MAX_BUF_LEN);
	Logger _log;
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

template<class Pack, class Logger, class... Dispatchers>
using TcpServer = 
	PreactorServer<tcp_preactor_t
		,net::Acceptor<MyConnection<Pack, Logger, Dispatchers...> >
	>;
NAMESP_END

#endif/*TCP_SERVER_H*/
