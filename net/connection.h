//connection.h

#ifndef CONNECTION_H
#define CONNECTION_H

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
#endif /*CONNECTION_H*/

