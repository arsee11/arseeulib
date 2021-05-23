
#include "../flexible_buffer.h"


#include <iostream>

using namespace std;
using namespace arsee;

int main()
{
	FlexibleBuffer<char> fb(100);
	cout<<"capacity="<<fb.capacity()<<endl;
	cout<<"size="<<fb.size()<<endl;

	cout<<"*********************************************"<<endl;
	const char* data="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX";
	fb.push(data, 50);
	cout<<"capacity="<<fb.capacity()<<endl;
	cout<<"size="<<fb.size()<<endl;

	cout<<"*********************************************"<<endl;
	char* tdata=nullptr;
	size_t size=0;
	std::tie(tdata, size)=fb.head(50);
	cout<<"head(50):"<<size<<", data:"<<tdata<<endl;
	std::tie(tdata, size)=fb.head(60);
	cout<<"head(60):"<<size<<", data:"<<tdata<<endl;

	cout<<"*********************************************"<<endl;
	cout<<"cunsume(40):"<<fb.consume(40)<<endl;
	cout<<"capacity="<<fb.capacity()<<endl;
	cout<<"size="<<fb.size()<<endl;
	std::tie(tdata, size)=fb.head(60);
	cout<<"head(60):"<<size<<", data:"<<tdata<<endl;

	cout<<"*********************************************"<<endl;
	cout<<"cunsume(40):"<<fb.consume(40)<<endl;
	cout<<"capacity="<<fb.capacity()<<endl;
	cout<<"size="<<fb.size()<<endl;
	std::tie(tdata, size)=fb.head(60);
	cout<<"head(60):"<<size<<", data:"<<tdata<<endl;
	
	cout<<"*********************************************"<<endl;
	fb.clear();
	cout<<"capacity="<<fb.capacity()<<endl;
	cout<<"size="<<fb.size()<<endl;
	std::tie(tdata, size)=fb.head(60);
	cout<<"head(60):"<<size<<", data:"<<tdata<<endl;
	
	cout<<"*********************************************"<<endl;
	fb.push(data, 50);
	cout<<"capacity="<<fb.capacity()<<endl;
	cout<<"size="<<fb.size()<<endl;
	fb.push(data, 50);
	cout<<"capacity="<<fb.capacity()<<endl;
	cout<<"size="<<fb.size()<<endl;
	fb.push(data, 50);
	cout<<"capacity="<<fb.capacity()<<endl;
	cout<<"size="<<fb.size()<<endl;
	std::tie(tdata, size)=fb.head(fb.size());
	cout<<"head:"<<size<<", data:"<<tdata<<endl;
	
	return 0;

}
