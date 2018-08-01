//@file executable_queue.h

#ifndef EXECUTABLE_QUEUE_H
#define EXECUTABLE_QUEUE_H

#include <queue>
#include <functional>
#include <mutex>

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif


NAMESP_BEGIN

typedef std::function<void()> exec_object_t;

class ExecutableQueue
{
public:
    ExecutableQueue()
    {
    }

    void post(exec_object_t& f)
    {
        std::lock_guard<std::mutex> g(_mutex);
        _exec_queue.push(f);
        _condv.notify_all();
    }

    void post(exec_object_t&& f)
    {
        std::lock_guard<std::mutex> g(_mutex);
        _exec_queue.push(f);
        _condv.notify_all();
    }
    
    void clear(){ post(nullptr); }
    void exec()
    {
        exec_object_t func = nullptr;
        std::unique_lock<std::mutex> lock(_mutex);
            _condv.wait(lock, [this]{ return _exec_queue.size()>0;});
            func = _exec_queue.front();
            _exec_queue.pop();
        lock.unlock();

        if(func != nullptr)
            func();
    }
    
    size_t size(){ 
        std::lock_guard<std::mutex> g(_mutex);
        return _exec_queue.size();
    }

private:
    std::queue<exec_object_t> _exec_queue;
    std::mutex _mutex;
    std::condition_variable _condv;
};
NAMESP_END
#endif // EXECUTABLE_QUEUE_H
