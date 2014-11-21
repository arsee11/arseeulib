//guarders_test.cpp
#include "guarders.h"
#include <thread>
#include <iostream>
#include <fstream>

#if defined(__GNUC__)
	#include <unistd.h>
#elif defined(_MSC_VER)
	#include <windows.h>
#endif 
	
using namespace std;

int i=0;
typename mutex_guard_t::lockable_t lock;
//critical_section_guard_t::lockable_t lock;


fstream fs("r.log", ios::out);

void f(int id)
{

	while(i<1000)
	{
		{
			mutex_guard_t g(&lock);
			//critical_section_guard_t g(&lock);
			cout << "id:" << id << endl;
			i++;
			fs << 1234 << ":"<<i << endl;
		}
		
		#if defined(__GNUC__)
		usleep(rand()%100*1000);
		#elif defined(_MSC_VER)
		Sleep(rand()%100);
		#endif
	}
};

void test_guard()
{
	thread t1(f,1);
	thread t2(f, 2);
	thread t3(f, 3);
	t1.join();
	t2.join();
	t3.join();
	cout<<"end"<<endl;
}
