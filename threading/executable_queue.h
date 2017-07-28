//@file executable_queue.h

#include <queue>
#include <functional>
#include <mutex>

typedef std::founction<void()> exec_object_t;

class ExecutableQueue
{

public:
	ExecutableQueue()
	{
	}

	void post(exec_object_t& f)
	{
		std::lock_guard<std::mutex> g(_mutex);
		_work_queue.push(f);
	}

	void post(exec_object_t&& f)
	{
		std::lock_guard<std::mutex> g(_mutex);
		_work_queue.push(f);
	}
  
 	void exec()
  {
        exec_object_t func = nullptr;
        _mutex.lock();
        if(_work_queue.size() > 0)
        {
          func = _work_queue.front();
          _work_queue.pop();
        }
        _mutex.unlock();

        if(func != nullptr)
          func();
  }
  
private:
	std::queue<exec_object_t> _exec_queue;
	std::mutex _mutex;
};
