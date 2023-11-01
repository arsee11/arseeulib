#ifndef THREAD_H
#define THREAD_H

#include <thread>
#include <memory>

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif


NAMESP_BEGIN

template<class Queue>
class Thread
{
    typedef Thread<Queue> my_t;
public:
    using thread_id = std::thread::id;

public:
    Thread(Queue* q)
        :_queue(q)
    {
        _thread = std::unique_ptr<std::thread>( new std::thread(
                std::bind(&my_t::exec, this))
        );
    }

    void stop(){
        _is_stop = true;
	    _queue->clear();
        _thread->join();
    }

    bool is_running()const { return !_is_stop; }
    void join(){try{_thread->join();}catch(...){} }
	thread_id getId()const{ return _thread->get_id(); }
    void setName(const std::string& name){ 
#ifdef __GNUC__
        pthread_setname_np(_thread->native_handle(), name.c_str());
#endif
    }
    std::string getName()const{ 
#ifdef __GNUC__
        char buffer[16];
        pthread_getname_np(_thread->native_handle(), buffer, 16);
        return std::string(buffer);
#else
        return "";
#endif
    }
	
	//return current thread's id
	static thread_id get_curid(){ return std::this_thread::get_id(); }
	
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

template<class Queue>
class CurThread
{
public:
    using thread_id = std::thread::id;

public:
    CurThread(Queue* q)
        :_queue(q)
    {
    }

    void stop(){
        _is_stop = true;
	    _queue->clear();
    }

	thread_id getId(){ return std::this_thread::get_id(); }
	
    void exec(){
        while(!_is_stop)
        {
            _queue->exec();
        }
    }

private:
    Queue* _queue=nullptr;
    volatile bool _is_stop = false;
};
NAMESP_END
#endif // THREAD_H
