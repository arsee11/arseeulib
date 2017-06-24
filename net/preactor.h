//proactor.h 
//copyright	:Copyright (c) 2014 arsee.
//license	:GNU GPL V2.

#ifndef ROACTOR_H
#define ROACTOR_H

#include <algorithm>
#include <list>
#include <iostream>
#include <unistd.h>

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

using namespace std;

NAMESP_BEGIN
namespace net
{

//////////////////////////////////////////////////////////////////////////////
template<class Selector, class EventQueueHolder>
class Proactor
{
	typedef Selector selector_t; 
	typedef Proactor
public:
	Preactor(size_t max):_selector(max){}
	
	void run()
	{
//		cout<<"evnet loop..."<<endl;
		while(true)
		{
			netevent_set_t events =  _selector.Select();
			if( n = 0 )
				continue;
			
			for(auto event : events)
			{
		        	event.setListener(this);
				event.wakeup();
			}			
		}
	}

	void onInput(fd_t fd)
	{
		char* buf = new char[MAX_BUF_LEN];
		size_t rlen = read(fd, buf, MAX_BUF_LEN);
		if( rlen > 0 )
		{
			RecvDataCompletedEvent e(fd, buf, rlen);
			_event_queue.push(e);	
			_event_queue.wakeup();
		}
	}
	
	void onOutput(fd_t, fd)
	{
		SendDataCompletedEvent e(fd);
		_event_queue.push(e);	
		_event_queue.wakeup();
	}
	
	void PostSend(int fd)
	{
		_selector.ModifySend(fd);
	}

	void PostRecv(int fd)
	{
		_selector.ModifyRecv(fd);
	}

	void PostClose(int fd)
	{
		_selector.Detach(fd);
		_fdhldr.Detach(fd);
		close(fd);
	}

protected:
	selector_t _selector;
	EventQueueHolder _event_queue;	
};

}//net
NAMESP_END
#endif /*ROACTOR_H*/
