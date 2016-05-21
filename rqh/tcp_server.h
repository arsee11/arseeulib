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

	void InputHandle(size_t len)
	{
#ifdef DEBUG
		cout<<"recv("<<len<<"):"<<this->_inbuf+8<<endl;
#endif
		pack_t pck;
		_userial(pck, this->_inbuf, len);
		if(pck.status() )
		{
			RequestContext context = {this->_fd, this->_remoteip, this->_remote_port};
			typename pack_t::pack_list_t pcks;
			try{
				DispatcherHandler<Dispatchers...>::Handle(context, pck);
			}
			catch(dispatcher_notfound_exp& e){
				_log.add(e.what(), __FILE__, __LINE__);
			}

			for(auto &i : pcks)
				_replies.push_back(i);
			
		}		
		else
		{
			_log.add("pack error", __FILE__, __LINE__);
		}
	}
	void OutputHandle()
	{
#ifdef DEBUG
		_log.add("OutputHandle()",__FILE__, __LINE__);
#endif
		vector<const char*> bufs;
		vector<size_t> bufsizes;
		for (auto &ip : _replies)
		{
			size_t bufsize=0;
			typename pack_t::serial_t serial;
			bufs.push_back( serial(ip, &bufsize) );
			bufsizes.push_back(bufsize);
			this->_outbuf_size += bufsize;
		}
		
		if( this->_outbuf != nullptr)
		{
			delete[] this->_outbuf;
			this->_outbuf=nullptr;
		}
		this->_outbuf = new char[this->_outbuf_size];
		for(size_t i=0; i<bufs.size(); i++)
		{
			if(i==0)
				memcpy(this->_outbuf, bufs[0], bufsizes[0]); 
			else
				memcpy(this->_outbuf+bufsizes[i-1], bufs[i], bufsizes[i]); 

			delete[] bufs[i];
		}

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
