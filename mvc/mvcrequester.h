//mvcrequester.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-24
//record 
//****************************

#ifndef MVC_REQUESTER_H
#define MVC_REQUESTER_H

#include <string>
#include <exception>


#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif


#ifndef JPACK_H
#include "jpack.h"
#endif

#ifndef TCP_SOCK_H 
#include "../net/tcpsock.h"
#endif

#include <thread>

NAMESP_BEGIN

class rqtexcpt:public std::exception
{
public:
	rqtexcpt(const char* str)
		:_str(std::string("rqtexcpt")+str)
	{
	}

#if defined(__GNUC__)
	const char* what()const noexcept override
#else
	const char* what()const override
#endif
	{
		return _str.c_str();
	}

private:
	std::string _str;
};


//@Sockptr must be a smart pointer
template<class Sockptr, class Pack>
class RequesterTmp
{
	typedef Pack pack_t;

public:
	void sender(const Sockptr &s)const{ _sock = s; }
	RequesterTmp()=default;
	virtual ~RequesterTmp(){ _listen_status = false; if(_listener_thread.joinable()) _listener_thread.join(); };

	void source(const std::string& val){ _source = val; }
	void source(std::string&& val){ source(val); }
	void source(const char* val){ source(std::string(val)); }

	void action(const std::string& val){ _action = val; }
	void action(std::string&& val){ action(val); }
	void action(const char* val){ action(std::string(val)); }
	
	void param(const std::string &name, const std::string& value){ _pack.param(name, value); }
	void param(std::string &&name, const std::string& value){ param(name, value); }
	void param(const char* name, const std::string& value){ param(std::string(name), value); }
	void param(std::string &&name, std::string&& value){ param(name, value); }
	void param(const std::string &name, std::string&& value){ param(name, value); }
	void param(const char* name, std::string&& value){ param(string(name), value); }
	void param(std::string &&name, const char* value){ param(name, std::string(value)); }
	void param(const std::string &name, const char* value){ param(name, std::string(value)); }
	void param(const char* name, const char* value){ param(std::string(name), std::string(value)); }

	//@timeout seconds.
	std::string Request(int timeout = -1)throw(rqtexcpt)
	{
		char rbuf[1024]={0};
		int r = Request(rbuf, 1024, timeout);
		if (r>0)
			return std::move(string(rbuf + pack_t::HeadField + pack_t::LenField));
			
		return "not recv";
	}
	
	template<class Response>
	void Request(Response &rsp, int timeout=-1)
	{
		char rbuf[1024]={0};
		//ToDo:do until a whole pack or timeout
		int r = Request(rbuf, 1024, timeout);
		if (r>0)
			rsp.Parse(rbuf, r);
		//else
			//rsp.Status(false);
	}

	template<class View>
	void AddListener(View &v)
	{
	}

	void Close(){ if(_sock!=nullptr) _sock->Close(); }
	//ToDo:Id handle
	void GenerateId(){ _id = "0"; }

private:
	//@timeout seconds.
	int Request(char *rbuf, int rlen, int timeout = -1)throw(rqtexcpt)
	{
		if(_sock == nullptr)
			throw rqtexcpt("not open!");

		GenerateId();
		typename pack_t::serial_t ss;
		size_t len=0;
		_pack.action(_action);
		_pack.source(_source);
		_pack.param("rqt_id", _id);
		const char* buf = ss(_pack, &len);
		_sock->Write(buf, len);
		_pack.Reset();
		
		return _sock->Read(rbuf, rlen, timeout);
	}
	
protected:
	pack_t _pack;
	std::string _action;
	std::string _source;
	std::string _id;
	Sockptr _sock = nullptr;
	std::thread _listener_thread;
	bool _listen_status = true;
};


////////////////////////////////////////////////////////////////////////////////
template<class Pack>
class TcpRequester:
	public RequesterTmp<TcpSock::rpeer_ptr_t, Pack>
{
	typedef RequesterTmp<TcpSock::rpeer_ptr_t, Pack> base_t;
public:
	TcpRequester()=default;
	~TcpRequester(){ base_t::Close(); }

	void Open(const char* rip, unsigned short rport)throw(sockexcpt)
	{
		base_t::_sock = TcpSock::CreateClient(std::string(rip), rport);
	}
};


typedef TcpRequester<Jpack> JTcpRequester;

NAMESP_END

#endif/*MVC_REQUESTER_H*/
