//requester.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-24
//record 
//****************************

#ifndef REQUESTER_H
#define REQUESTER_H

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
	void sender(const Sockptr &s){ _sock = s; }
	Sockptr sender()const{ return _sock ; }

	RequesterTmp()
	{
		_pack.type("request");
	}

	virtual ~RequesterTmp(){ _listen_status = false; if(_listener_thread.joinable()) _listener_thread.join(); };

	void source(const std::string& val){ _source = val; }
	void source(std::string&& val){ source(val); }
	void source(const char* val){ source(std::string(val)); }

	void set_continue(const std::string& val){ _pack.set_continue(val); }
	void set_continue(std::string&& val){ set_continuea(val); }
	void set_continue(const char* val){ set_continue(std::string(val)); }
	
	void add_param(const std::string &name, const std::string& value){ _pack_item[name]=value; }
	void add_param(std::string &&name, const std::string& value){ add_param(name, value); }
	void add_param(const char* name, const std::string& value){ add_param(std::string(name), value); }
	void add_param(std::string &&name, std::string&& value){ add_param(name, value); }
	void add_param(const std::string &name, std::string&& value){ add_param(name, value); }
	void add_param(const char* name, std::string&& value){ add_param(string(name), value); }
	void add_param(std::string &&name, const char* value){ add_param(name, std::string(value)); }
	void add_param(const std::string &name, const char* value){ add_param(name, std::string(value)); }
	void add_param(const char* name, const char* value){ add_param(std::string(name), std::string(value)); }
	
	void append_param(){ _pack.append_param(_pack_item); _pack_item.clear(); }
	
	//@timeout seconds.
	std::string Request()throw(rqtexcpt)
	{
		char rbuf[1024]={0};
		int r = RequestNoWait(rbuf, 1024);
		if (r>0)
			return std::move(string(rbuf + pack_t::HeadField + pack_t::LenField));
			
		throw rqtexcpt("request failed!");
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

	int RequestNoWait(char *rbuf, int rlen)throw(rqtexcpt)
	{
		if (_sock == nullptr)
			throw rqtexcpt("not open!");

		GenerateId();
		typename pack_t::serial_t ss;
		size_t len = 0;
		_pack.action(_action);
		_pack.source(_source);
		typename pack_t::param_item_t pitem;
		pitem["rqt_id"] = _id;
		_pack.append_param(pitem);
		const char* buf = ss(_pack, &len);
		_sock->Write(buf, len);
		cout << buf + 8 << endl;
		_pack.Reset();	
	}
	
protected:
	pack_t _pack;
	typename pack_t::param_item_t _pack_item;
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
	public RequesterTmp<net::TcpSock::rpeer_ptr_t, Pack>
{
	typedef RequesterTmp<net::TcpSock::rpeer_ptr_t, Pack> base_t;
public:
	TcpRequester()=default;
	~TcpRequester(){ base_t::Close(); }

	void Open(const char* rip, unsigned short rport)throw(net::sockexcpt)
	{
		base_t::_sock = net::TcpSock::CreateClient(std::string(rip), rport);
	}

	void Open(const string& rip, unsigned short rport)throw(net::sockexcpt)
	{
		return Open(rip.c_str(), rport);
	}
};


typedef TcpRequester<Jpack> JTcpRequester;

NAMESP_END

#endif/*REQUESTER_H*/
