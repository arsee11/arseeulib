#ifndef THREAD_H
#define THREAD_H

#include <thread>
#include <memory>

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif


NAMESP_BEGIN

using thread_id = std::thread::id;

template<class Queue>
class Thread
{
    typedef Thread<Queue> my_t;

public:
    Thread(Queue* q)
        :_queue(q)
    {
        _thread = std::unique_ptr<std::thread>( new std::thread(
                  std::bind(&my_t::exec, this)));
    }

    void stop(){
        _is_stop = true;
	_queue->clear();
	cout<<"join.. "<<_queue->size()<<endl;
        _thread->join();
    }

    void join(){ _thread->join(); }

	thread_id getId(){ return _thread->get_id(); }
	
private:
    void exec(){
        while(!_is_stop)
        {
            _queue->exec();
        }
    }

private:
    Queue* _queue=nullptr;
    std::unique_ptr<std::thread> _thread;
    volatile bool _is_stop = false;
};

NAMESP_END
#endif // THREAD_H
