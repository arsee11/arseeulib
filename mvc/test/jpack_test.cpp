//test_jpack.cpp

#include "../jpack.h"
#include<iostream>



using namespace arsee;
using namespace std;


//jpack_test -I/home/arsee_p/lib/jsoncpp-master/include/ -L/home/arsee_p/lib/jsoncpp-master/lib/\
-std=c++11 jpack_test.cpp ../mvclib/jpack.cpp -ljsoncpp 

//normal
void test_serializ1()
{
	cout<<"test_serializ1:"<<endl;
	Jpack pck("aaa", "bbb","ccc");
	Jpack::param_item_t param_item;
	param_item["ddd"] = "1111";
	param_item["eee"] = "eee";
	Jpack::params_pack_t pparam;
	pparam.push_back(param_item);
	
	Jpack::serial_t s;
	size_t len;
	const char *buf = s(pck, &len);
	cout<<"result("<<len<<"):";
	cout<<"head:"<<*(long*)buf<<"len:"<<*(long*)(buf+sizeof(long))<<"payload:"<<buf+2*sizeof(long)<<endl;
}

//no source
void test_serializ2()
{
	cout<<"test_serializ2:"<<endl;
	Jpack pck("", "bbb","ccc");
	Jpack::param_item_t param_item;
	param_item["ddd"] = "1111";
	param_item["eee"] = "eee";
	Jpack::params_pack_t pparam;
	pparam.push_back(param_item);
	
	Jpack::serial_t s;
	size_t len;
	const char *buf = s(pck, &len);
	cout<<"result("<<len<<"):";
	cout<<"head:"<<*(long*)buf<<"len:"<<*(long*)(buf+sizeof(long))<<"payload:"<<buf+2*sizeof(long)<<endl;
}

//unserial
void test_unserializ()
{
	cout<<"test_unserializ:"<<endl;
	Jpack pck("aaa", "bbb","ccc");
	Jpack::param_item_t param_item;
	param_item["ddd"] = "1111";
	param_item["eee"] = "eee";
	Jpack::params_pack_t pparam;
	pparam.push_back(param_item);
	
	Jpack::serial_t s;
	size_t len;
	const char *buf = s(pck, &len);

	Jpack::unserial_t us(1024);
	Jpack npack;
	int r = us(npack, buf, len);
	cout<<"result("<<r<<"):"<<endl;
	cout<<"action:"<<npack.Action()<<endl;
	cout<<"source:"<<npack.Source()<<endl;
	cout<<"target:"<<npack.Target()<<endl;
	for(auto &i:npack.Params())
		for(auto &j:i)
		 cout<<j.first<<"="<<j.second<<endl;
	
}

int main()
{
	test_serializ1();
	test_serializ2();
	test_unserializ();
	return 0;
}

