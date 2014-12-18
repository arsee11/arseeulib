//test.cpp

#include <iostream>
#include <strings.h>
#include <string.h>
#include <memory>
#include <string>
#include "preactor.h"
#include "selector.h"
#include "session.h"
#include "acceptor.h"

using namespace std;

typedef Preactor<FdHolder, true, Epoll> tcp_preactor_t;


typedef Acceptor< Session<1024, tcp_preactor_t> > acceptor_t;
typedef acceptor_t::ss_container_t ss_container_t    
ss_container_t _sessions;

class Server
{
public:
	Server()
	{
		_reactor.RegistryAcceptor(new acceptor_t(11111, &_sessions));
	}

	int Run()
	{
		_reactor.EventLoop();
	}
	

private:
	tcp_preactor_t _reactor = tcp_preactor_t(1024);
};


void t(void *arg)
{
	while(true)
	{
		cout<<"t..."<<endl;
		ss_container_t *ss = static_cast<ss_container_t*>(arg);
		ss_container_t::iterator i = ss->begin();
		for(; i!=ss->end(); ++it)
			(*i)->PostOutput("hello", 6);
		
		sleep(1);
	}
}

int main( )
{
	pthread_t fd;
	pthread_create(&fd, NULL, t, &sessions);
	Server svr_instance;
	svr_instance.Run();

	return 0;
};
