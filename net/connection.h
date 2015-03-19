//connection.h
//copyright	:Copyright (c) 2014 arsee.
//license	:GNU GPL V2.

#ifndef CONNECTION_H
#define CONNECTION_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

class Conntection
{
public:
	void open()throw(sockexp);
	void close()throw(sockexp);
	int read()throw(sockexp);
	int write()throw(sockexp);	

private:
	unique_ptr<ConnState> _state;
};


class ConnState
{
public:
	virtual void open()throw(sockexp);
	virtual void close()throw(sockexp);
	virtual int read()throw(sockexp);
	virtual int write()throw(sockexp);	

};


class WaitingState
{
public:
	override void open()throw(sockexp);
	override void close()throw(sockexp);
	override int read()throw(sockexp);
	override int write()throw(sockexp);	

};


class OpenedState
{
public:
	override void open()throw(sockexp);
	override void close()throw(sockexp);
	override int read()throw(sockexp);
	override int write()throw(sockexp);	

};


class ClosedState
{
public:
	override void open()throw(sockexp);
	override void close()throw(sockexp);
	override int read()throw(sockexp);
	override int write()throw(sockexp);	

};

}//net
NAMESP_END

#endif /*CONNECTION_H*/

