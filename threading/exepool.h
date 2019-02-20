//task_pool.h
//
//

#ifndef TASK_POOL_H
#define TASK_POOL_H

#include <memory>
#include <list>
#include <threading/executable_queue.h>
#include <threading/thread.h>
#include <threading/dispatcher.h>

template<class Scope>
class TaskPool
{



using scope_ptr = std::shared_ptr<Scope>; 
using scope_list = std::list<Scope>; 

public:
	TaskPool(int num_of_scopes=1){
		for(int i=0; i<num_of_threads; i++)
		{
			_scopes.push_back(taskqueue_ptr(new TaskQueue()));
		}
	}


	scope_ptr getOne(){
		if(scopes.size() == 0 )
			return nullptr;

		if(_scopes.size() == 1 )
			return *_scopes.begin();


		//find the task most empty task queue.
		auto first = _scopes.begin();
		 std::list<taskqueue_ptr>::iterator found=first;
		
		int size = (*first)->_queue_size();
		++first;
		for(; first != _scopes.end(); ++first)
		{
			if(size > (*first)->queue_size() )
			{
				found = first;
				size = (*first)->queue_size();
			}

		}

		return *found;
	}
	
private:
	scope_list _scopes;	
};

#endif /*TASK_POOL_H*/
