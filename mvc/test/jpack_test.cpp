//test_jpack.cpp

#include "../jpack.h"
#include<iostream>

#if defined(_MSC_VER)
#pragma comment(lib, "../../lib/jsonlibd")
#endif

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
	pck.append_param(param_item);
	Jpack::serial_t s;
	size_t len;
	const char *buf = s(pck, &len);
	cout<<"result("<<len<<"):";
	cout<<"head:"<<*(int*)buf<<"len:"<<*(long*)(buf+sizeof(long))<<"payload:"<<buf+2*sizeof(long)<<endl;
}

//no source
void test_serializ2()
{
	cout<<"test_serializ2:"<<endl;
	Jpack pck("", "bbb","ccc");
	Jpack::param_item_t param_item;
	param_item["ddd"] = "1111";
	param_item["eee"] = "222";
	pck.append_param(param_item);

	Jpack::param_item_t param_item2;
	param_item2["ddd"] = "333";
	param_item2["eee"] = "444";
	pck.append_param(param_item2);
	
	Jpack::serial_t s;
	size_t len;
	const char *buf = s(pck, &len);
	cout<<"result("<<len<<"):";
	cout<<"head:"<<*(int*)buf<<"len:"<<*(int*)(buf+sizeof(int))<<"payload:"<<buf+2*sizeof(int)<<endl;
}

//unserial
void test_unserializ()
{
	cout<<"test_unserializ:"<<endl;
	Jpack pck("aaa", "bbb","ccc");
	Jpack::param_item_t param_item;
	param_item["ddd"] = "1111";
	param_item["eee"] = "222";
	pck.append_param(param_item);
	
	Jpack::param_item_t param_item2;
	param_item2["ddd"] = "333";
	param_item2["eee"] = "444";
	pck.append_param(param_item2);

	Jpack::serial_t s;
	size_t len;
	const char *buf = s(pck, &len);

	Jpack::unserial_t us(1024);
	Jpack npack;
	int r = us(npack, buf, len);
	cout<<"result("<<r<<"):"<<endl;
	cout<<"action:"<<npack.action()<<endl;
	cout<<"source:"<<npack.source()<<endl;
	cout<<"target:"<<npack.target()<<endl;
	for (auto &i : npack.params())
	{
		for (auto &j : i)
			cout << j.first << "=" << j.second << endl;
	}
		 
	
}

void test_userialize_2()
{
	string payload="{\"action\":\"trans_msg\",\"source\":\"msgview\",\"target\":\"msg\",\"params\":{\"param0\":[{\"name\":\"msg\",\"value\":\"hello\"},{\"name\":\"from\",\"value\":\"1\"},{\"name\":\"to\",\"value\":\"2\"}]}}";
	
	char buf[1024]={0};
	buf[0] = 0xff;
	buf[1] = 0xff;
	buf[2] = 0xff;
	buf[3] = 0xff;

	size_t len =  payload.size();
	memcpy(buf+4, &len, 4);
	memcpy(buf+8, payload.c_str(), len);
	Jpack::unserial_t us(1024);
	Jpack npack;
	int r = us(npack, buf, len+8);
	cout<<"result("<<r<<"):"<<endl;
	cout<<"action:"<<npack.action()<<endl;
	cout<<"source:"<<npack.source()<<endl;
	cout<<"target:"<<npack.target()<<endl;
	for (auto &i : npack.params())
	{
		for (auto &j : i)
			cout << j.first << "=" << j.second << endl;
	}
		 

}

int main()
{
	//test_serializ1();
	//test_serializ2();
	//test_unserializ();
	test_userialize_2();
	return 0;
}

