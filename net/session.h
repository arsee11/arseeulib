//session.h

#ifndef SESSIOIN_H
#define SESSIOIN_H

#include <iostream>
#include <strings.h>
#include <string.h>
#include <memory>
#include <string>
#include <map>

#ifndef FD_DEF_H
#include "fddef.h"
#endif

#ifndef EVENTS_H
#include "events.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

struct Key{
	std::string ip;
	unsigned short port;
};

inline bool operator<(const Key& lhs, const Key& rhs)
{
	if( lhs.ip < rhs.ip )
		return true;
		
	if( lhs.ip>rhs.ip )
		return false;
		
	if(lhs.ip == rhs.ip )
		return lhs.port<rhs.port;
}


template<class SessionT>
class SessionContainer
{
public:
	//typedef std::shared_ptr<SessionT> session_ptr_t; 
	typedef SessionT session_t; 
	typedef session_t* session_ptr_t; 
	typedef Key key_t;
	typedef typename std::map<key_t, session_ptr_t>::iterator iterator;

public:
	static SessionContainer<SessionT>& instance()
	{
		static SessionContainer<SessionT> _myself;
		return _myself;
	}

	session_ptr_t get(const key_t& key)
	{
		return _sessions[key];
	}
	
	session_ptr_t get(const key_t&& key)
	{
		return _sessions[key];
	}
	
	session_ptr_t get(const std::string& ip, unsigned short port)
	{
		key_t key = {ip, port};
		return _sessions[key];
	}
	
	session_ptr_t get(const std::string&& ip, unsigned short port)
	{
		key_t key = {ip, port};
		return _sessions[key];
	}

	void put(SessionT *ss)
	{
		key_t key = {ss->remote_ip(), ss->remote_port()};
		_sessions[key] = session_ptr_t(ss);
	}

	void pop(const SessionT *ss)
	{
		key_t key = {ss->remote_ip(), ss->remote_port()};
		_sessions.erase(key);
	}

	iterator begin(){ return _sessions.begin();}
	iterator end(){ return _sessions.end();}

private:
	SessionContainer(){}
	~SessionContainer(){}
	SessionContainer(SessionContainer&);
	SessionContainer& operator=(SessionContainer&);

	std::map<key_t, session_ptr_t> _sessions;
};

struct Buf
{
	Buf(const char* buf, size_t len);
	Buf(const char* buf);
	
	~Buf(){ delete[] buf; }
	
	char *buf=nullptr;
	size_t size=0;
};

typedef std::shared_ptr<Buf> buf_t;

template<size_t insize, class Preactor, class Derived>
class Session
{
public:
	//typedef SessionContainer<Session<insize, Preactor> > ss_container_t;
	typedef SessionContainer<Derived> ss_container_t;

public:
	Session(fd_t fd, std::string remoteip, unsigned short remote_port)
		:_remoteip(remoteip)
		,_remote_port(remote_port)
		,_fd(fd)
	{
	//	ss_container_t::instance().put(this);
	}

	Session()=delete;

	void preactor(Preactor *p)
	{
		_preactor = p;
	}

	virtual ~Session()
	{
#ifdef DEBUG
		cout<<"~Session"<<endl;
#endif
//		ss_container_t::instance().pop(this);
	}

	virtual void InputHandle(size_t len)
	{

#ifdef DEBUG
		cout << "read("<<len<<"):"<< _inbuf <<endl;
#endif
	}

	virtual void OutputHandle()
	{
#ifdef DEBUG
		cout<<"out handling..."<<endl;
		char *outbuf = new char[6];
		strcpy(outbuf, "hello, ");
		_outbuf = outbuf;
		_outbuf_size = 6;
#endif
	}

	virtual void CloseHandle()
	{
#ifdef DEBUG
		cout<<_remoteip<<":"<<_remote_port<<"disconnected"<<endl;
#endif
	}

	int PostOutput(const char* buf, size_t size)
	{
		_outbuf = const_cast<char*>(buf);
		_outbuf_size = size;
		_preactor->PostSend(_fd); 
	}

	fd_t fd()const {return _fd;}
	std::string remote_ip()const { return std::move(_remoteip); }
	unsigned short remote_port()const { return _remote_port; }
	
	char *inbuf(){ return _inbuf; }
	size_t in_bufsize(){ return insize; }

	const char *outbuf(){ return _outbuf; }
	size_t out_bufsize(){ return _outbuf_size; }
	void ClearOutbuf(){ _outbuf = nullptr; _outbuf_size=0;}

protected:
	std::string _remoteip;
	unsigned short _remote_port = -1;
	fd_t _fd;
	char _inbuf[insize] = {0};
	char *_outbuf = nullptr;
	size_t _outbuf_size = 0;
	Preactor *_preactor = nullptr;
};

}//net
NAMESP_END

#endif /*SESSIOIN_H*/
