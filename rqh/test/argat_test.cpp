//argat_test.cpp

#include "../utility.h"

#include <string>
#include <iostream>
#include <typeinfo>

using namespace std;

int main()
{
	cout<<typeid(typename ArgAt<0, int, float, double>::result).name()<<endl;
	cout<<typeid(typename ArgAt<1, int, float, double>::result).name()<<endl;
	cout<<typeid(typename ArgAt<2, int, float, double>::result).name()<<endl;
}
