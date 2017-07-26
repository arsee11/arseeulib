//selector.h

#ifndef SELECTOR_H	
#define SELECTOR_H	

#include <sys/epoll.h>
#include <fcntl.h>
#include <exception>
#include <iostream>

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

using namespace std;

NAMESP_BEGIN
namespace net
{

template<class SelectedHandler>
class Selector
{
public:
	Selector(size_t max)
		:_selector(max)
	{
	}
	
	//@hldr return the alivable fds container,
	//@return number of alivable fds.
	template<class Holder>
	size_t Select(Holder &hldr)
	{
#ifdef DEBUG
		cout<<"selecting..."<<endl;
#endif
		
		netevent_set_t events = _selector.Select();
		for(auto i : events)
		{
			hldr->SendEvent(i);
		}

		return events->size();
	}

private:
	SelectedHandler _selector;
};

}//net
NAMESP_END

#endif /*SELECTOR_H*/	
