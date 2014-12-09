//selector.h

#ifndef SELECTOR_H	
#define SELECTOR_H	

#include <hash_map>
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

using namespace std;


void SetNoblocking(fd_t fd)
{
	int flag = fcntl(fd, F_GETFD, NULL);
	if( fcntl(fd, F_SETFL, flag|O_NONBLOCK) == -1 )
	{
		perror("fcntl");
		throw exception();
	}
}

class EpollBase
{
public:
	EpollBase(size_t max)
		:_max(max)
	{
		Init();
	}

	virtual ~EpollBase()
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

	size_t Select(epoll_event *ehs)
	{
#ifdef DEBUG
		cout<<"EpollBase::selecting..."<<endl;
#endif
		int nfds = epoll_wait(_efd, ehs, _max, -1);
		if(nfds == -1)
		{
			//error handle
			perror("epoll_wait");
			return 0;
		}

		return nfds;
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

template<bool tcp=false>
class Epoll
{
};

template<>
class Epoll<true>:
	public EpollBase
{
public:
	Epoll(size_t max)
		:EpollBase(max)
	{
	}

	void SetAcceptor(int accptfd)
	{
		_accptfd = accptfd;
	}

	//@hldr return the alivable fds container,
	//@return number of alivable fds.
	template<class Holder>
	size_t Select(Holder &hldr)
	{
#ifdef DEBUG
		cout<<"selecting..."<<endl;
#endif
		epoll_event ehs[_max];
		size_t nfds = EpollBase::Select(ehs);
		for(size_t i=0; i<nfds; ++i)
		{
			if(ehs[i].events & EPOLLIN )
			{
				if(ehs[i].data.fd == _accptfd)
					hldr.Turn2Accept(ehs[i].data.fd);
				else					
					hldr.Turn2In(ehs[i].data.fd);
			}

			if(ehs[i].events & EPOLLOUT )
				hldr.Turn2Out(ehs[i].data.fd);
		}

		return nfds;
	}

private:
	int _accptfd = -1;
};

#endif /*SELECTOR_H*/	
