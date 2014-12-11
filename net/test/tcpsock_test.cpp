//tcpsock_test.cpp

#ifndef TCPSOCK_H
#include "../tcpsock.h"
#endif

#include <iostream>

using namespace arsee;
using namespace std;

int main()
{
	SockInit();
	
	string buf;
	TcpSock::rpeer_ptr_t peer;

	try{
		peer = TcpSock::CreateClient("127.0.0.1", 11111);
	}
	catch(sockexcpt &e){
		perror(e.what());
		return 0;
	}

	while( buf != "q")
	{
		cout<<"input something(q to quit):";
		cin>>buf;
		int len = peer->Write(buf.c_str(), buf.size());
		cout <<len<<" bytes sent"<<endl;
		char rbuf[11]={0};
		len = peer->Read(rbuf, 10);
		cout <<len<<" bytes recv:"<<rbuf<<endl;
	}
	
	cout<<"bye"<<endl;
	SockUninit();
	
}
