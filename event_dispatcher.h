///@file event_dispatcher.h

#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

NAMESP_BEGIN

template<class EventHandler>
class EventDispatcher
{
public:
  void addEventHandler(EventHandler* h){ _event_handlers.push_back(h); }

	template<class Event>
	void sendEvent(const Event& evt){
		event_handler_list_t::iterator it = _event_handlers.begin();
		for(; it != _event_handlers.end(); ++it)
			evt.fire( *it );
	}

private:
	typedef std::vector<EventHandler*> event_handler_list_t;
	event_handler_list_t _event_handlers;
};

NAMESP_END/*namespace*/

#endif /*EVENT_DISPATCHER_H*/
