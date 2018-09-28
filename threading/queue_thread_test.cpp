#include <iostream>
#include <thread>

#include "executable_queue.h"
#include "thread.h"

using namespace std;
using namespace arsee;

int main()
{
	//multi threads witdh one queue
	ExecutableQueue q;
	Thread<ExecutableQueue> eth(&q);
	Thread<ExecutableQueue> eth2(&q);
	Thread<ExecutableQueue> eth3(&q);
	Thread<ExecutableQueue> eth4(&q);
	
	bool running=true;
	thread th([&q, &running](){ 
		while(running)
		{
			q.post( [](){
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				cout<<std::this_thread::get_id()<<" call obj"<<endl;
			});

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	});

	
	getchar();
	running = false;
	th.join();
	eth.stop();
	eth2.stop();
	eth3.stop();
	eth4.stop();

	return 0;
}
