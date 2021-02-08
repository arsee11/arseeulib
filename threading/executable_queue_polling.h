///event_queue.h
//
//

#ifndef EXECUTABLE_QUEUE_POLLING_H
#define EXECUTABLE_QUEUE_POLLING_H

#include <memory>
#include <iostream>

NAMESP_BEGIN

template<class Poller>
class ExecutableQueuePolling 
{ 
public:
	ExecutableQueuePolling()
	{
	}

	~ExecutableQueuePolling()
	{
		std::cout<<"~NetExecutableQueuePollingBasic()"<<std::endl;
	}

    void process();

	void bind(fd_t fd, event_ptr e);

	void unbind(fd_t fd);

private:
	void process(event_ptr e){ e->fire(); }

private:
	Poller<Event> _poller;
};

NAMESP_END


#endif /*EXECUTABLE_QUEUE_POLLING_H*/
