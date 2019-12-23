#include "../executable_queue.h"
#include "../thread.h"
#include "../dispatcher.h"

#include <iostream>

using namespace std;
using namespace arsee;

void f(int i)
{
    cout<<"f:"<<"i="<<i<<endl;
}

void g(int i, const char* a)
{
    std::this_thread::sleep_for(std::chrono::seconds(2));
    cout<<"g:"<<"i="<<i<<",a="<<a<<endl;
}

struct A{
    float m(float a, float b)
    {
	    return a*b;
    }
};

int add(int a, int b)
{
    return a+b;
}

int main(int argc, char *argv[])
{
    ExecutableQueue q1, q2;
    Thread<ExecutableQueue> t1( &q1 );
    Thread<ExecutableQueue> t2( &q2 );

    dispatch_sync(&q1, &f, 10);

    dispatch_asyn(&q2, g, 11, "abc");

    A a;
    float s=dispatch_invoke(&q1, &A::m, &a, 2.0f, 3.0f);
    cout<<"m(2.0, 3.0):"<<s<<endl;

    int sum=dispatch_invoke(&q2, &add, 2, 3);
    cout<<"add(2, 3):"<<sum<<endl;

    t1.join();
}
