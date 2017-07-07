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
	typedef Proactor<selector_t, EventQueueHolder> my_t;
	
public:
	Preactor(size_t max):_selector(max){
		_event_queue.addFilter( std::bind(&my_t::filterEvent, this, _1) );
	}
	
	void setEventListener(EventListener* el){
		_event_queue.setListener(el);
	}
	
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
		        	_event_queue.push(e);	
				_event_queue.wakeup();
			}			
		}
	}
	
private:
	bool filterEvent(Event* e){
		if(e->type() == InputEvent::type )
		{
			onInput(e);	
			return true;
		}
		
		return false;
	}
	
	void onInput(InputEvent* e)
	{
		InputEvent::sender* s = e->sender();
		char* buf = new char[MAX_BUF_LEN];
		size_t rlen = s->read(buf, MAX_BUF_LEN);
		if( rlen > 0 )
		{
			RecvDataCompletedEvent e(s, buf, rlen);
			_event_queue.push(e);	
			_event_queue.wakeup();
		}
	}
	
	void onOutput(OutputEvent* e)
	{
		SendDataCompletedEvent se( e->sender() );
		_event_queue.push(se);	
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
