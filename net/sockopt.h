//file:tcpsock.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify	
//data	: 2015-3-17
//log	: record 
//****************************

////////////////////////////////
//Socket Option operations.
//@Sock the socket class.
struct SockOpt
{
	bool ReuseAddr()
	{
		sock.sock();
	}

	SOCKET _sock;
};