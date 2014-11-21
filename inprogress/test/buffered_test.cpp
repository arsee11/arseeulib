#include <thread>
#include <iostream>
#include <fstream>
#include <queuebuf.h>
#include <guarders.h>
#include <string>

#if defined(__GNUC__)
#include <time.h>

unsigned long GetTickCount()
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
#endif

struct obj
{
	obj()
	{
		cout << "constructor" << endl;
	}

	~obj()
	{
		cout << "disconstructor" << endl;
	}

	obj(const obj&)
	{
		cout << "copy constructor" << endl;
	}

	obj& operator=(const obj& rhs)
	{
		cout << "assign" << endl;
		i = rhs.i;
		return (*this);
	}

	obj& operator=(const obj&& rhs)
	{
		cout << "move assign" << endl;
		i = rhs.i;
		return (*this);
	}

	int i = 0;

};

QueueBuffer<char*, critical_section_guard_t > buf(10);

critical_section_guard_t::lockable_t glck;
int gi = 0;

void productor(int id)
{
	char fname[256];
	sprintf_s(fname, "%dp.log", id);
	fstream f(fname, ios::out);
	//while (true)
	{
		for (int i=0; gi < 200;i++)
		{
			char str[10] = { 0 };
			sprintf(str,"%d",gi);
			cout << "p:" << endl;
			//obj o;
			//o.i = 111;
			buf.Push(str, strlen(str));
			f << ::GetTickCount() << ":" << str << endl;
			Sleep(rand() % 10);

			critical_section_guard_t g(&glck);
			gi++;
		}
		
	}
}

bool state = true;
void consumer(int id)
{
	char fname[256];
	sprintf_s(fname, "%d.log", id);
	fstream f(fname, ios::out);
	int i = 0;
	vector<string> dumps;

	while (state)
	{
		char *str=nullptr;
		//f << id << ":Getting" << endl;
		int r = -1;
		cout << "c:" << endl;

		//obj o;
		buf.Pop(&str);
		//dumps.push_back(str);
		//f << id << ":Got" << endl;
		f <<::GetTickCount()<<":"<< str << std::endl;
		i++;
		Sleep(rand() % 50);
	}

	for (auto &i:dumps)
		f <<::GetTickCount()<<":"<< i << std::endl;
	
}

void test_start()
{
	try{
		std::thread tp1(productor, 1);
		//std::thread tp2(productor, 2);
		std::thread tc1(consumer, 1);
		std::thread tc2(consumer, 2);
		//std::thread tc3(consumer, 3);
		tp1.join();
		//tp2.join();
		//Sleep(5000);
		//state = false;
		//buf.WakeUp();
		tc1.join();
		//tc2.join();
		//tc3.join();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;

	}
	
	
}