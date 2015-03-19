//fd.h
//copyright	:Copyright (c) 2014 arsee.
//license	:GNU GPL V2.

#ifndef FD_H
#define FD_H

#include <utility>
#include <ext/hash_map>
#include <list>
#include <string>
#include <exception>

#ifndef EVENTS_H
#include "events.h"
#endif

#ifndef FD_DEF_H 
#include "fddef.h"
#endif

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

class FdExp:
	public std::exception
{
public:
	FdExp(const char* str)
		:_str(str)
	{
		_str = "FdExp:"+_str;
	}

	const char* what()const noexcept
	{
		return _str.c_str();
	}

private:
	std::string _str;
};

template<bool evt_accept, bool evt_in=true, bool evt_out=true, bool evt_close=true>
class Manager;

class Fd
{
	friend class Manager<false>;
	friend class Manager<true,false,false,false>;

public:
	typedef Event::evt_ptr_t evt_ptr_t;

public:
	Fd(fd_t fd)
		:_fd(fd)
	{
	}

	~Fd()
	{
//		cout<<"~Fd"<<endl;
	}

	void Accept()
	{
		_evt_state = _evts[EVT_ACCEPT];
	}
	
	void In()
	{
		_evt_state = _evts[EVT_IN];
	}

	
	void Out()
	{
		_evt_state = _evts[EVT_OUT];
	}
	
	void Close()
	{
		_evt_state = _evts[EVT_CLOSE];
	}

	void Wakeup() throw(FdExp)
	{
		if(_evt_state == nullptr )
		{
			//errorhandle
			throw FdExp("Fd state empty");
		}
		_evt_state->Notify();
	}

private:
	__gnu_cxx::hash_map<int, evt_ptr_t> _evts;

	evt_ptr_t _evt_state = nullptr;

	fd_t _fd = -1;

};


class FdHolder
{
	friend class Manager<false>;
	friend class Manager<true,false,false,false>;

public:
	typedef std::shared_ptr<Fd> fd_ptr_t;
	typedef std::list<fd_ptr_t> fd_list_t;
	typedef __gnu_cxx::hash_map<fd_t, fd_ptr_t> fd_container_t;

	typedef Manager<false> normal_manager_t ;
	typedef Manager<true,false,false,false> accept_manager_t ;

public:
	void Turn2Accept(fd_t fd)
	{
		auto rfd = _fd_cont.find( fd );  
		if( rfd != _fd_cont.end() );
		{
			rfd->second->Accept();
			_fd_lst.push_back(rfd->second);	
		}
	}

	void Turn2In(fd_t fd)
	{
		auto rfd = _fd_cont.find( fd );  
		if( rfd != _fd_cont.end() );
		{
			rfd->second->In();
			_fd_lst.push_back(rfd->second);	
		}
	}

	void Turn2Out(fd_t fd)
	{
		auto rfd = _fd_cont.find( fd );  
		if( rfd != _fd_cont.end() );
		{
			rfd->second->Out();
			_fd_lst.push_back(rfd->second);	
		}
	}

	void Turn2Close(fd_t fd)
	{
		auto rfd = _fd_cont.find( fd );  
		if( rfd != _fd_cont.end() );
		{
			rfd->second->Close();
			_fd_lst.push_back(rfd->second);	
		}
	}

	inline fd_list_t GetAlive()
	{
		return std::move(_fd_lst);
		_fd_lst.clear();
	}

	inline void Detach(fd_t fd)
	{
		_fd_cont.erase(fd);
	}

private:
	fd_list_t _fd_lst;
	fd_container_t _fd_cont;
};

template<bool evt_accept, bool evt_in, bool evt_out, bool evt_close>
class Manager
{
public:
	template<class Handler, class Preactor> 
	static inline void Attach(FdHolder &fdh, fd_t fd, std::shared_ptr<Handler>& h, Preactor *pr)
	{
		typedef Event::evt_ptr_t evt_ptr_t;
		typedef InputEvent<Handler, Preactor> in_evt_t;
		typedef OutputEvent<Handler, Preactor> out_evt_t;
		typedef CloseEvent<Handler, Preactor> close_evt_t;
		typename FdHolder::fd_ptr_t pfd = typename FdHolder::fd_ptr_t( new Fd(fd) );
		pfd->_evts[EVT_IN]	= evt_ptr_t(new in_evt_t(fd, h, pr));
		pfd->_evts[EVT_OUT]	= evt_ptr_t(new out_evt_t(fd, h, pr));
		pfd->_evts[EVT_CLOSE] 	= evt_ptr_t(new close_evt_t(fd, h, pr));
	
		fdh._fd_cont[fd] = pfd; 
	}
};

template<>
class Manager<true,false,false,false>
{
public:
	template<class Handler, class Preactor> 
	static inline void Attach(FdHolder &fdh, fd_t fd, Handler* h, Preactor *pr)
	{
		typedef Event::evt_ptr_t evt_ptr_t;
		typedef AcceptEvent<Handler, Preactor> accept_evt_t;
		typedef typename accept_evt_t::handler_ptr_t hptr_t;
		typename FdHolder::fd_ptr_t pfd = typename FdHolder::fd_ptr_t( new Fd(fd) );
		pfd->_evts[EVT_ACCEPT] = evt_ptr_t(new accept_evt_t(fd, hptr_t(h), pr));
	
		fdh._fd_cont[fd] = pfd; 
	}
};

}//net
NAMESP_END

#endif /*FD_H*/
