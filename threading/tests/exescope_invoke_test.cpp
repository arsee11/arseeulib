//tcp_svr_test.cpp

#include "exescope.h"

#include <iostream>
#include <stdio.h>

using namespace arsee;
using namespace std;
using namespace std::placeholders;

int add(int a, int b){
    return a+b;
}
int main()
{
    ExeScope exe;

    int c = exe.invoke(add, 1, 2);
    cout<<"c="<<c<<endl;
    std::string str = exe.invoke([]{
                    std::this_thread::sleep_for(1s);
                    return "hello"; });
    cout<<"str="<<str<<endl;

    exe.stop();
    std::string str2 = exe.invoke([]{
                    std::this_thread::sleep_for(1s);
                    return "hello2"; });

    cout<<"str2="<<str2<<endl;
    exe.stop();

}
