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


class Server
{
	typedef Acceptor< Session<1024> > acceptor_t;
public:
	Server()
	{
		_reactor.RegistryAcceptor(new acceptor_t("", 11111));
	}

	int Run()
	{
		_reactor.EventLoop();
	}


private:
	tcp_preactor_t _reactor = tcp_preactor_t(1024);
};


int main()
{
	Server svr_instance;
	svr_instance.Run();

	return 0;
};
