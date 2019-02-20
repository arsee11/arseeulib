//task_pool.cpp
//
//

#include "task_pool.h"

TaskPool::TaskQueue::TaskQueue()
{
	Q = new Queue;
	T = new Thread(Q);
}

TaskPool::TaskQueue::~TaskQueue()
{
	if(T!=nullptr)
	{
		T->stop();
		delete T;
	}

	if(Q!=nullptr)
		delete Q;
}

TaskPool::TaskPool(int num_of_threads)
{
	for(int i=0; i<num_of_threads; i++)
	{
		_task_queues.push_back(taskqueue_ptr(new TaskQueue()));
	}
}

TaskPool::taskqueue_list::iterator TaskPool::getTaskQueue()
{
	if(_task_queues.size() == 0 )
		return _task_queues.end();

	if(_task_queues.size() == 1 )
		return _task_queues.begin();


	//find the task most empty task queue.
	auto first = _task_queues.begin();
	 std::list<taskqueue_ptr>::iterator found=first;
	
	int size = (*first)->size();
	++first;
	for(; first != _task_queues.end(); ++first)
	{
		if(size > (*first)->size() )
		{
			found = first;
			size = (*first)->size();
		}

	}

	return found;
}
