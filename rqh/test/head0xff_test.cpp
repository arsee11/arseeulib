//head0xff_test.cpp

#include "../pack_2.h"
#include <iostream>
#include <string.h>

using namespace std;
using namespace arsee;

int main()
{
	char stream[3]={0xff,0xff, 0xff};
	size_t hlen=0;
	BufferWrapper buf(128);
	buf.put(stream, 3);
	bool ok= arsee::Head0xff(buf, &hlen);
	cout<<ok<<endl;
	
	char stream1[4]={0xff,0xff, 0xff, 0xff};
	BufferWrapper buf1(128);;
	buf1.put(stream1, 4);
	ok = arsee::Head0xff(buf1, &hlen);
	cout<<ok<<endl;

	char stream2[9]={0xff,0xff, 0xff, 0xff,'a','b','c','d'};
	BufferWrapper buf2(128);;
	buf2.put(stream2, 9);
	ok = arsee::Head0xff(buf2, &hlen);
	cout<<ok<<endl;

	char stream3[9]={'e',0xff,0xff, 0xff, 0xff,'a','b','c'};
	BufferWrapper buf3(128);;
	buf3.put(stream3, 9);
	ok = arsee::Head0xff(buf3, &hlen);
	cout<<ok<<endl;


	return 0;
}
