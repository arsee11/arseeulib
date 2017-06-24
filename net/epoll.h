//epoll.h

#ifndef EPOLL_H	
#define EPOLL_H	

#include <memory>
#include <sys/epoll.h>
#include <fcntl.h>
#include <exception>
#include <iostream>

#ifndef FD_H
#include "fd.h"
#endif

#ifndef FD_DEF_H 
#include "fddef.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

using namespace std;

NAMESP_BEGIN
namespace net
{

inline void SetNoblocking(fd_t fd)
{
	int flag = fcntl(fd, F_GETFD, NULL);
	if( fcntl(fd, F_SETFL, flag|O_NONBLOCK) == -1 )
	{
		perror("fcntl");
		throw std::exception();
	}
}

class Epoll
{
public:
	Epoll(size_t max)
		:_max(max)
	{
		Init();
	}

	virtual ~Epoll()
	{
	}
	
	void Attach(fd_t fd)
	{
		if(_nfds == _max)
		{
			//error handle
		}

		SetNoblocking(fd);
		epoll_event eh;
		eh.events = EPOLLET|EPOLLIN;//always in event.
		eh.data.fd = fd;
		if( epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &eh) == -1)
		{
			//error handle
			perror("epoll_ctl");
		}

		++_nfds;
	}

	void Detach(fd_t fd)
	{
		if( epoll_ctl(_efd, EPOLL_CTL_DEL, fd, NULL) == -1)
		{
			//error handle
		}

		_nfds>0 ? 0 : --_nfds;
	}

	netevent_set_t Select(netevent_set_t& ehs)
	{
#ifdef DEBUG
		cout<<"EpollBase::selecting..."<<endl;
#endif
		epoll_event ehs[_max];
		int nfds = epoll_wait(_efd, ehs, _max, -1);
		if(nfds == -1)
		{
			//error handle
			perror("epoll_wait");
			return 0;
		}

		netevent_set_t events;
		for(size_t i=0; i<nfds; ++i)
		{			
			if(ehs[i].events&EPOLLIN || ehs[i].events&EPOLLPRI)
			{
				NetInputEvent event(ehs[i].data.fd);
				events.push_back(event);
			}
			else if(ehs[i].events&EPOLLOUT )
			{
				NetOutputEvent event(ehs[i].data.fd);
				events.push_back(event);
			}
			else if(ehs[i].events&EPOLLRDHUP )
			{
				NetCloseEvent event(ehs[i].data.fd);
				events.push_back(event);
			}
			else
			{
			}
		}
		return std::move(events);
	}

	void ModifySend(fd_t fd)
	{
		epoll_event tmp;
		tmp.data.fd = fd;
		tmp.events = EPOLLET|EPOLLOUT;
		if( epoll_ctl(_efd, EPOLL_CTL_MOD, fd, &tmp)==-1 ) 
		{
			//error handle
			perror("epoll_ctl");
		}

	}

	void ModifyRecv(fd_t fd)
	{
		epoll_event tmp;
		tmp.data.fd = fd;
		tmp.events = EPOLLET|EPOLLIN;
		if( epoll_ctl(_efd, EPOLL_CTL_MOD, fd, &tmp)==-1 ) 
		{
			//error handle
			perror("epoll_ctl");
		}
	}

private:
	void Init()
	{
		_efd = epoll_create(_max);
		if( _efd == -1 )
		{
			//ErrorHandle();
			perror("epoll_create");
			throw std::exception();
		}
	}

protected:
	size_t _max;

private:
	size_t _nfds;
	fd_t _efd;

};

}//namespace net
#endif /*EPOLL_H*/
