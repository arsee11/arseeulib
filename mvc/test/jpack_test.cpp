//test_jpack.cpp

#include "../jpack.h"
#include<iostream>



using namespace arsee;
using namespace std;

//-I/home/arsee_p/lib/jsoncpp-master/include/

//-l/home/arsee_p/lib/jsoncpp-master/lib/

//normal
void test_serializ1()
{
	cout<<"test_serializ1:"<<endl;
	Jpack pck("aaa", "bbb","ccc");
	pck.Param("ddd", "1111");
	pck.Param("eee", "2222");
	
	Jpack::serial_t s;
	size_t len;
	const char *buf = s(pck, &len);
	cout<<"result("<<len<<"):";
	cout<<"head:"<<*(long*)buf<<"len:"<<*(long*)(buf+sizeof(long))<<"payload:"<<buf+2*sizeof(long)<<endl;
}

//not source
void test_serializ2()
{
	cout<<"test_serializ2:"<<endl;
	Jpack pck("", "bbb","ccc");
	pck.Param("ddd", "1111");
	pck.Param("eee", "2222");
	
	Jpack::serial_t s;
	size_t len;
	const char *buf = s(pck, &len);
	cout<<"result("<<len<<"):";
	cout<<"head:"<<*(long*)buf<<"len:"<<*(long*)(buf+sizeof(long))<<"payload:"<<buf+2*sizeof(long)<<endl;
}
int main()
{
	test_serializ1();
	test_serializ2();
	return 0;
}

