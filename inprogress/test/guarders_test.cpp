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

void f(int id)
{
	while(i<10000)
	{
		cout<<"id:"<<id<<endl;
		{
			mutex_guard_t g(&lock);
			i++;
			cout<<i<<endl;
		}
		
		#if defined(__GNUC__)
		usleep(rand()%50*1000);
		#elif defined(_MSC_VER)
		Sleep(rand()%50);
		#endif
	}
};

void test_guard()
{
	thread t1(f,1);
	thread t2(f,2);
	t1.join();
	t2.join();
	cout<<"end"<<endl;
}
