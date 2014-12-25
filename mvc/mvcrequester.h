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


//@Sender must smart pointer
template<class Sender, class Pack>
class RequesterTmp
{
	typedef Pack pack_t;

public:
	void sender(const Sender &s)const{ _sender = s; }
	RequesterTmp()=default;
	virtual ~RequesterTmp(){};

	void Action(const std::string& action){ _pack.Action(action); _pack.Reset(); }
	void Action(std::string&& action){ Action(action); }
	void Action(const char* action){ Action(std::string(action)); }
	
	void Param(const std::string &name, const std::string& value){ _pack.Param(name, value); }
	void Param(std::string &&name, const std::string& value){ Param(name, value); }
	void Param(const char* name, const std::string& value){ Param(std::string(name), value); }
	void Param(std::string &&name, std::string&& value){ Param(name, value); }
	void Param(std::string &&name, const char* value){ Param(name, std::string(value)); }
	void Param(const char* name, const char* value){ Param(std::string(name), std::string(value)); }

	void Request(std::string &msg, int timeout=-1)throw(rqtexcpt)
	{
		if(_sender == nullptr)
			throw rqtexcpt("not open!");

		GenerateId();
		typename pack_t::serial_t ss;
		size_t len=0;
		const char* buf = ss(_pack, &len);
		_sender->Write(buf, len);
		//ToDo:do until a whole pack or timeout
		char rbuf[1024]={0};
		_sender->Read(rbuf, 1024, timeout);
	}

	void Close(){ if(_sender!=nullptr) _sender->Close(); }
	//ToDo:Id handle
	void GenerateId(){ _id = "0"; }

protected:
	pack_t _pack;
	std::string _action;
	std::string _id;
	Sender _sender=nullptr;
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
		base_t::_sender = TcpSock::CreateClient(std::string(rip), rport);
	}
};


typedef TcpRequester<Jpack> JTcpRequester;

NAMESP_END

#endif/*MVC_REQUESTER_H*/
