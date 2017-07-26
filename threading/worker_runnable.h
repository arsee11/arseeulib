//@file worker_runnable.h

#include <queue>
#include <functional>
#include <mutex>


class WorkerRunnable
{
public:
	WorkerRunnable()
	{
	}

	void work()
	{
		while( !_is_stop )
		{
			std::function<void()> func = nullptr;
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
	}
	
	void stop(){ _is_stop = true;}

	template<class Function>
	void post(Function& f)
	{
		std::lock_guard<std::mutex> g(_mutex);
		_work_queue.push(f);
	}

	template<class Function>
	void post(Function&& f)
	{
		std::lock_guard<std::mutex> g(_mutex);
		_work_queue.push(f);
	}

private:
	std::queue<std::function<void()>> _work_queue;
	std::mutex _mutex;
	bool _is_stop = false;
};
