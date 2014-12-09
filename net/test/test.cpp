//test.cpp

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <memory>
#include <string>
#include "reactor.h"
#include "events.h"
#include "selector.h"

using namespace std;
typedef EpollSelector<Event> selector_t;
typedef typename selector_t::event_ptr_t event_ptr_t;
typedef Reactor<EpollSelector<Event> > reactor_t;

class Client
{
	typedef InputEvent<Client, reactor_t> input_evt_t;
	typedef OutputEvent<Client, reactor_t> output_evt_t;
	typedef CloseEvent<Client> close_evt_t;
	typedef shared_ptr<input_evt_t> input_evt_ptr_t;
	typedef shared_ptr<output_evt_t> output_evt_ptr_t;
	typedef shared_ptr<close_evt_t> close_evt_ptr_t;

public:
	vector<event_ptr_t> _evts;
	Client(int fd, string remoteip, unsigned short remote_port)
		:_remoteip(remoteip)
		,_remote_port(remote_port)
	{
		_evts.push_back( input_evt_ptr_t(new input_evt_t(fd, this)) );
		_evts.push_back( output_evt_ptr_t(new output_evt_t(fd, this)) );
	//	_evts.push_back( close_evt_ptr_t(new close_evt_t(fd, this)) );
	}

	
	event_ptr_t FirstEvent(){ _evtidx = 0; return _evts[0] ; }
	event_ptr_t NextEvent()
	{
		if(++_evtidx>=_evts.size())
			return nullptr;
		
		return _evts[ _evtidx ];
	}

	void InputHandle(int fd)
	{
		char buf[1024]={0};	
		int len =0;
		while( (len = read(fd, buf, 1) )>0 )
		{
			cout<<_remoteip<<":"<<_remote_port<<" "<<buf<<endl;
		}

		if(len==-1 & errno==EAGAIN)
		{
			cout<<"..."<<endl;
			len=	write(fd, "hi", 2);
			cout<<"write("<<len<<")"<<endl;
		}
		if(len==0)
			cout<<"close"<<endl;
	}

	void OutputHandle(int fd)
	{
		cout<<"out handling..."<<endl;
		char *buf="I know you.";
		int len = write(fd, buf, strlen(buf) ); 

	}

private:
	string _remoteip;
	unsigned short _remote_port;
	int _evtidx = 0;
};

class Listener;

class Server
{
public:
	void SetListener(shared_ptr<Listener>&& l)
	{
		_listener = l;
		_reactor.RegistryHandler(l);
	}

	int Run()
	{
		_reactor.EventLoop();
	}

	void AddConn(int fd, sockaddr_in *addr)
	{
		shared_ptr<Client> cli = shared_ptr<Client>( new Client(fd, 
			string(inet_ntoa(addr->sin_addr)), 
			ntohs(addr->sin_port) )
		);
		
		_conns.push_back(cli); 
		_reactor.RegistryHandler(cli);
	}

private:
	shared_ptr<Listener> _listener;

	reactor_t _reactor;
	list<shared_ptr<Client> > _conns;
};

Server svr_instance;

class Listener
{
	typedef InputEvent<Listener, reactor_t> input_evt_t;
	typedef shared_ptr<input_evt_t> input_evt_ptr_t;
public:
	vector<event_ptr_t> _evts;
	Listener()
	{
		Create();
		_evts.push_back( input_evt_ptr_t(new input_evt_t(_fd,this)) );
	}

	event_ptr_t  FirstEvent(){ return (*_evts.begin() ); }
	event_ptr_t  NextEvent(){ return nullptr; };

	void Create()
	{
		sockaddr_in addr;
		bzero(&addr, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(11111);
		_fd = socket(AF_INET, SOCK_STREAM, 0);
		bind(_fd, (sockaddr*)&addr, sizeof(addr));
		listen(_fd, 5);
	}

	void InputHandle(int fd)
	{
		cout<<"accepting a new client..."<<endl;
		sockaddr_in addr;
		socklen_t addrlen = sizeof(addr);
		bzero(&addr, addrlen);
		int newfd = accept(fd, (sockaddr*)&addr, &addrlen);
		svr_instance.AddConn(newfd, &addr);

	}

private:
	int _fd;
};

int main()
{
	svr_instance.SetListener( shared_ptr<Listener>(new Listener) );
	svr_instance.Run();
	return 0;

};
