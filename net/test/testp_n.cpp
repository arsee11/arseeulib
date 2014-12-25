//test.cpp

#include <iostream>
#include <strings.h>
#include <string.h>
#include <memory>
#include <string>
#include "../preactor.h"
#include "../selector.h"
#include "../session.h"
#include "../acceptor.h"

using namespace std;

typedef Preactor<FdHolder, true, Epoll> tcp_preactor_t;

typedef Session<1024, tcp_preactor_t> session_t;
typedef Acceptor< session_t> acceptor_t;
typedef typename session_t::ss_container_t ss_container;

class Server
{
public:
	Server()
	{
		_reactor.RegistryAcceptor(new acceptor_t(11111));
	}

	int Run()
	{
		_reactor.EventLoop();
	}
	

private:
	tcp_preactor_t _reactor = tcp_preactor_t(1024);
};


void* t(void *arg)
{
	while(true)
	{
//		cout<<"t..."<<endl;
		ss_container::iterator i = ss_container::instance().begin();
		for(; i!=ss_container::instance().end(); ++i)
			i->second->PostOutput("hello", 6);
		
		sleep(1);
	}
}

int main( )
{
	pthread_t fd;
	pthread_create(&fd, NULL, t, NULL);
	Server svr_instance;
	svr_instance.Run();

	return 0;
};
