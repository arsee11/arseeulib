//@file worker_runnable.h

#include <queue>
#include <functional>

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
			if(_work_queue.size() > 0)
			{
				auto f = _work_queue.front();
				_work_queue.pop();
				f();
			}
		}
	}
	
	void stop(){ _is_stop = true;}

	template<class Function>
	void post(Function& f)
	{
		_work_queue.push(f);
	}

	template<class Function>
	void post(Function&& f)
	{
		post(f);
	}

private:
	std::queue<std::function<void()>> _work_queue;
	bool _is_stop = false;
};
