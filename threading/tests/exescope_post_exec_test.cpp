//tcp_svr_test.cpp

#include "exescope.h"

#include <iostream>
#include <stdio.h>

using namespace arsee;
using namespace std;
using namespace std::placeholders;

void  add(int a, int b){
    cout<<"a+b="<< a+b<<endl;
}

struct A
{
    void f(int n){
        cout<<"A::f\n";
    }
};
int main()
{
    ExeScope exe;

    exe.exec(add, 1, 2);
    exe.exec(std::bind(add, 1, 2));
    exe.exec([]{ cout<<"hello\n"; });

    A a;
    exe.post(&A::f, &a, 2);
    exe.post(add, 1, 2);
    exe.post(std::bind(add, 1, 2));
    exe.post([]{ cout<<"hello\n"; });

    getchar();
    exe.stop();

}
