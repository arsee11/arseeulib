//@file buffer_wrapper_test.cpp

#include "../buffer_wrapper.h"

#include <iostream>
#include <string.h>

using namespace std;
using namespace arsee;

void test_1()
{
	cout<<"******test_1 begin********"<<endl;
	char* inbuf = "12345678901234567890";
	BufferWrapper buf(94);
	for(int i=0; i<100001; i++)
	{
		buf.put(inbuf, strlen(inbuf));
		char outbuf[11]={0};
		buf.get(outbuf, 10);
		cout<<"outbuf:"<<outbuf<<endl;
		bool r = (strcmp(outbuf, "1234567890")==0);
		cout<<"result:"<<r<<endl;
		if(!r)
			break;

	}
	cout<<"******test_1 end********"<<endl;
}

void test_2()
{
	cout<<"******test_2 begin********"<<endl;
	char* inbuf = "12345678901234567890";
	BufferWrapper buf(94);
	for(int i=0; i<1000; i++)
	{
		buf.put(inbuf, strlen(inbuf));
		char outbuf[26]={0};
		buf.get(outbuf, 25);
		cout<<"outbuf:"<<outbuf<<endl;

	}
	cout<<"******test_2 end********"<<endl;
}

int main()
{
	//test_1();
	test_2();
	return 0;
}
