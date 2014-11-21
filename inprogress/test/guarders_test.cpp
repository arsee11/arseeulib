//guarders_test.cpp
#include "guarders.h"
#include <thread>
#include <iostream>
#include <fstream>

#if define(__GNUC__)
	#include <unistd.h>
#elif define(_MSC_VER)
	#include <windows.h>
#endif 
	
int i=0;
mutex_guard_t::lockable_t lock;

void f(int id)
{
	cout<<"id:"<<id<<endl;
	{
		mutex_guard_t g(&lock);
		i++;
		cout<<i<endl;
	}
	
	#if define(__GNUC__)
	usleep(rand()%100*1000);
	#elif define(_MSC_VER)
	Sleep(rand()%100);
	#endif
};

void test_guard()
{
	thread t1(1);
	thread t2(2);
	t1.jion();
}