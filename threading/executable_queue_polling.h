///event_queue.h
//
//

#ifndef EXECUTABLE_QUEUE_POLLING_H
#define EXECUTABLE_QUEUE_POLLING_H

#include <memory>
#include <iostream>
#include <atomic>

NAMESP_BEGIN


typedef std::function<void()> exec_object_t;

template<class Poller>
class ExecutableQueuePolling 
{ 
    struct Pipe{
        Pipe(){ pipe(fd); }
        void read() { ::read(fd[0], buf, 1);  }
        void write(){ ::write(fd[1], "p", 1); }
        int fd[2];
        char buf[1];
    };

public:
	void setPoller(Poller* poller)
	{
        _poller = poller;
        if(_poller != nullptr){
            _poller->bindInput(_pipe.fd[0], std::bind(&Pipe::read, &_pipe));
        }
	}

	~ExecutableQueuePolling()
	{
	}

    void post(exec_object_t& f)
    {
	    std::lock_guard<std::mutex> g(_mutex);
        _exec_queue.push(f);
        _pipe.write();
    }

    void post(exec_object_t&& f)
    {
         post(f);   
    }

    void clear(){ 
        _is_clear = true;
        _pipe.write();
	}

    void exec()
    {
        while(true){
            if(_is_clear)
                break;

            exec_object_t func = nullptr;
            {
                std::unique_lock<std::mutex> lock(_mutex);
	    	    if(_exec_queue.size() > 0){
                    func = _exec_queue.front();
            	    _exec_queue.pop();
                }
                else{
                    break;
                }
            }

            if(func != nullptr){
                std::unique_lock<std::mutex> lock(_mutex);
	    	    if(_exec_queue.size()==0){
                    lock.unlock();
                    func();
                    break;//if this is the last one
                }
                else{
                    func();
                }
            }

        }

        if(_poller != nullptr && !_is_clear){
            printf("process...\n");
            _poller->process();
        }
    }

    size_t size(){ 
        std::lock_guard<std::mutex> g(_mutex);
		return _exec_queue.size(); 
    }


private:
	Poller* _poller=nullptr;
    std::queue<exec_object_t> _exec_queue;
    std::mutex _mutex;
    std::atomic<bool> _is_clear;
    Pipe _pipe;
};


NAMESP_END


#endif /*EXECUTABLE_QUEUE_POLLING_H*/
