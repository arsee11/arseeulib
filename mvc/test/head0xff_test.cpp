//head0xff_test.cpp

#include "../pack.h"
#include <iostream>
#include <string.h>

using namespace std;

int main()
{
	char stream[3]={0xff,0xff, 0xff};
	size_t hlen=0;
	const char * buf = arsee::Head0xff(stream, 3, &hlen);
	cout<<(buf==nullptr)<<endl;
	
	char stream1[4]={0xff,0xff, 0xff, 0xff};
	buf = arsee::Head0xff(stream1, 4, &hlen);
	cout<<(buf==nullptr)<<endl;

	char stream2[9]={0xff,0xff, 0xff, 0xff,'a','b','c','d'};
	buf = arsee::Head0xff(stream2, 8, &hlen);
	cout<<(strcmp(buf,"abcd")==0)<<endl;

	char stream3[9]={'e',0xff,0xff, 0xff, 0xff,'a','b','c'};
	buf = arsee::Head0xff(stream3, 8, &hlen);
	cout<<(strcmp(buf,"abc")==0)<<endl;


	return 0;
}
