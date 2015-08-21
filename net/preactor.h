//preactor.h 
//copyright	:Copyright (c) 2014 arsee.
//license	:GNU GPL V2.

#ifndef REACTOR_H
#define REACTOR_H

#include <algorithm>
#include <list>
#include <iostream>
#include <unistd.h>

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

using namespace std;

NAMESP_BEGIN
namespace net
{

//////////////////////////////////////////////////////////////////////////////
template<class FdHolder
	,class Selector
>
class PreactorBase
{
	typedef Selector selector_t; 
	typedef FdHolder fd_holder_t;
	typedef typename fd_holder_t::fd_ptr_t fd_ptr_t;
	typedef typename fd_holder_t::fd_list_t fd_list_t;

public:
	PreactorBase(size_t max):_selector(max){}

	//@h Handler pointer it must be created from heap(use new operator).
	template<class Handler> 
	inline void RegistryHandler(shared_ptr<Handler> &h)
	{
		_selector.Attach( h->fd() );	
		FdHolder::normal_manager_t::Attach(_fdhldr, h->fd(), h, this );
	}

	template<class Handler> 
	inline void RemoveHandle(const Handler *h)
	{
		_selector.Detach( h->fd() );
		_fdhldr.Detach( h->fd() );
	}

	void EventLoop()
	{
//		cout<<"evnet loop..."<<endl;
		while(true)
		{
			size_t n =  _selector.Select(_fdhldr);
			if( n = 0 )
				continue;

			fd_list_t alive_fds = _fdhldr.GetAlive();
			for(auto &i:alive_fds)
			{
				i->Wakeup();
			}
		}
	}

	void PostSend(int fd)
	{
		_selector.ModifySend(fd);
	}

	void PostRecv(int fd)
	{
		_selector.ModifyRecv(fd);
	}

	void PostClose(int fd)
	{
		_selector.Detach(fd);
		_fdhldr.Detach(fd);
		close(fd);
	}

protected:
	selector_t _selector;
	fd_holder_t _fdhldr;	
};


//////////////////////////////////////////////////////////////////////////////
template<class FdHolder,
	bool tcp,
	template<bool> class SELECTOR
>
class Preactor;


//////////////////////////////////////////////////////////////////////////////
template<class FdHolder,
	template<bool>class  SELECTOR
>
class Preactor<FdHolder, true, SELECTOR> :
	public PreactorBase<FdHolder, SELECTOR<true> >
{
	typedef PreactorBase<FdHolder, SELECTOR<true> > base_t;

public:
	Preactor(size_t max)
		:base_t(max)
	{
	}

	//@h Aceeptor pointer it must be created from heap(use new operator).
	template<class Acceptor> 
	inline void RegistryAcceptor(Acceptor *h)
	{
		base_t::_selector.SetAcceptor( h->fd() );	
		base_t::_selector.Attach( h->fd() );	
		FdHolder::accept_manager_t::Attach(base_t::_fdhldr, h->fd(), h, this );
	}

};

}//net
NAMESP_END
#endif /*REACTOR_H*/
