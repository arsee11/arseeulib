//tcpsock.cpp

#ifndef TCPSOCK_H
#include <tcpsock.h>
#endif

rpeer_ptr_t TcpSock::CreateClient(const std::string &ip, unsigned short port) throw(std::exception)
{
	return  nullptr;
}

lpeer_ptr_t TcpSock::CreateServer(const std::string &ip, unsigned short port) throw(std::exception)
{
	int s = socket(AF_INET, SOCK_STREAM, 0);

}
