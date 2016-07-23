//events.h
//copyright	:Copyright (c) 2014 arsee.
//license	:GNU GPL V2.

#ifndef EVENTS_H
#define EVENTS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#ifndef FD_DEF_H
#include "fddef.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

//////////////////////////////////////////////////////////////////////////
class Event
{
public:
	typedef size_t type_t;
	typedef std::shared_ptr<Event> evt_ptr_t;

public:
	Event(fd_t fd)
		:_fd(fd)
	{
	}

	~Event()
	{
//		cout<<"~Event"<<endl;
	}

	virtual void Notify()=0;

	fd_t fd(){ return _fd; }

protected:
	//handler_ptr_t _handler;
	fd_t _fd;
};


//////////////////////////////////////////////////////////////////////////
//complete events.
//own the @HANDLER, manager its life time.
template<class HANDLER, class Preactor>
class AcceptEvent:
	public Event
{

public:
	typedef HANDLER handler_t;
	typedef std::shared_ptr<handler_t> handler_ptr_t;

public:
	AcceptEvent(fd_t fd, const handler_ptr_t& h, Preactor *pr)
		:Event(fd )
		,_handler(h)
		,_preactor(pr)
	{
	}

	~AcceptEvent()
	{
//		cout<<"~AcceptEvent"<<endl;
	}

	virtual void Notify() override
	{
#ifdef DEBUG
		cout<<"AcceptEvent::Notify"<<endl;
#endif
		sockaddr_in addr;
		bzero(&addr, sizeof(addr));
		socklen_t addl = sizeof(addr);
		fd_t newfd = accept(_fd, (sockaddr*)&addr, &addl); 
		typename handler_t::session_ptr_t s = _handler->AcceptHandle(newfd, 
			inet_ntoa(addr.sin_addr),
			ntohs(addr.sin_port)
		);

		s->preactor(_preactor);
		_preactor->RegistryHandler(s);
	}


private:
	handler_ptr_t _handler;
	Preactor *_preactor;
};


//////////////////////////////////////////////////////////////////////////
template<class HANDLER, class Preactor>
class InputEvent:
	public Event
{

public:
	typedef HANDLER handler_t;
	typedef std::shared_ptr<handler_t> handler_ptr_t;

public:
	InputEvent(fd_t fd, const handler_ptr_t& h, Preactor *pr)
		:Event(fd )
		,_handler(h)
		,_preactor(pr)
	{
	}

	~InputEvent()
	{
//		cout<<"~InputEvent"<<endl;
	}

	virtual void Notify() override
	{
#ifdef DEBUG
		cout<<"InpuEvent::Notify"<<endl;
#endif
		char *buf = _handler->inbuf();	
		int nwant = _handler->in_bufsize();
		int len =0;
		int tlen=0;
		while( (len = read(_fd, buf+tlen, nwant) )>0 )
		{
			tlen+=len;
		}

		if(len==-1 && errno==EAGAIN)
		{
			WorkerThreads::instance().post(std::bind(handler_t::InputHandle, _handler, len));
			_preactor->PostSend(_fd);
		}

		if(len==0)
			_preactor->PostClose(_fd);
	}

private:
	handler_ptr_t _handler;
	Preactor *_preactor;
};


//////////////////////////////////////////////////////////////////////////
template<class HANDLER, class Preactor>
class OutputEvent:
	public Event
{
public:
	typedef HANDLER handler_t;
	typedef std::shared_ptr<handler_t> handler_ptr_t;

	OutputEvent(fd_t fd, const handler_ptr_t& h, Preactor *pr)
		:Event(fd )
		,_handler(h)
		,_preactor(pr)
	{
	}

	~OutputEvent()
	{
//		cout<<"~OutputEvent"<<endl;
	}

	virtual void Notify()
	{
#ifdef DEBUG
		cout<<"OutpuEvent::Notify"<<endl;
#endif
		_handler->OutputHandle();
		const char *buf = _handler->outbuf();
		size_t len = _handler->out_bufsize();
		if( len > 0 )
		{
			cout<<_fd<<":write{"<<len<<"):"<<buf+8<<endl;
			int r = write(_fd, buf, len);
			_handler->ClearOutbuf();
		}

		_preactor->PostRecv(_fd);
	}


private:
	handler_ptr_t _handler;
	Preactor *_preactor;
};


//////////////////////////////////////////////////////////////////////////
template<class HANDLER, class Preactor>
class CloseEvent:
	public Event
{
public:
	typedef HANDLER handler_t;
	typedef std::shared_ptr<handler_t> handler_ptr_t;

	CloseEvent(fd_t fd, const handler_ptr_t& h, Preactor *pr)
		:Event(fd )
		,_handler(h)
		,_preactor(pr)
	{
	}

	~CloseEvent()
	{
//		cout<<"~CloseEvent"<<endl;
	}

	virtual void Notify()
	{
#ifdef DEBUG
		cout<<"CloseEvent::Notify"<<endl;
#endif
		_handler->CloseHandle();
		_preactor->PostClose(_fd);
	}

private:
	handler_ptr_t _handler;
	Preactor *_preactor;
};

}//net
NAMESP_END

#endif /*EVENTS_H*/
