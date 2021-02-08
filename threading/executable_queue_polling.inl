///event_queue.inl
//
//

NAMESP_BEGIN
{

template<
	template<class> class Poller
>
void EventQueue<Poller>::bind(fd_t fd, event_ptr e){
	_poller.addInput(fd, e);
}

template<
	template<class> class Poller
>
void EventQueue<Poller>::unbind(fd_t fd){
}

template<
	template<class> class Poller
>
void EventQueue<Poller>::process(){
	auto events = _poller.select();
	for(auto e : events)
	{
		process(e);
	}
}

NAMESP_END

