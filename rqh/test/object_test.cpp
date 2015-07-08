//object_test.cpp

#include "../mvcobject.h"
#include <string>
#include <iostream>

using namespace std;
using namespace arsee;

struct I
{
	static string name(){ return "int"; }
	int i;
};


struct F
{
	static string name(){ return "float"; }
	float f;
};

int main()
{
	
	typedef ObjectsCollection<I,F> oct;
	I i;
	F f;
	oct::Init(&i, &f);
	I *ii = oct::Instance().GetObj<I>();
	cout<<"expt:"<<&i<<" act:"<<ii<<endl;
	F *ff = oct::Instance().GetObj<F>();
	cout<<"expt:"<<&f<<" act:"<<ff<<endl;
}
