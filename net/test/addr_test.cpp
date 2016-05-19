//#include "../addr.h"
#include "../../mvc/server.h"
//#ifndef ACCEPTOR_H
//#include "..//acceptor.h"
//#endif

#include <iostream>

using namespace std;

int main()
{
	arsee::net::SockConfig conf(1, 22, "1.", "2.");
	cout<<conf.lport<<" "<<conf.rport<<" "<<conf.lip<<" "<<conf.rip<<endl;
}
