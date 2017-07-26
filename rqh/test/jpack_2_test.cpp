//test_jpack.cpp

#include "../jpack.h"
#include<iostream>
#include<fstream>
#include <string>

#if defined(_MSC_VER)
#pragma comment(lib, "../../lib/jsonlibd")
#endif

#ifndef CLASS_INFO_H
#include "../class_info.h"
#endif

using namespace arsee;
using namespace std;

DEF_CLASS_INHERIT_BEGIN(MyObject, IObject)
	MyObject()
	{
		REGISTER_ATTR(MyObject, int,    a);
		REGISTER_ATTR(MyObject, float,  b);
		REGISTER_ATTR(MyObject, string, c);
	}
	MyObject& operator+=(const IObject& rhs){ return *this;}
	const char* get_class_name(){ return class_info.get_class_name();}
	BUILD_ATTR(int,    a);
	BUILD_ATTR(float,  b);
	BUILD_ATTR(string, c);
DEF_CLASS_END(MyObject)

DEF_CLASS_NAME(MyObject);

//jpack_test -I/home/arsee_p/lib/jsoncpp-master/include/ -L/home/arsee_p/lib/jsoncpp-master/lib/\
//-std=c++11 jpack_2_test.cpp ../mvclib/jpack2.cpp -ljsoncpp 

void print_pack(Jpack& npack)
{
	cout << "action:" << npack.action() << endl;
	cout << "source:" << npack.source() << endl;
	cout << "target:" << npack.target() << endl;
	cout << "num of objects:" << npack.object_list().size() << endl;
	for (auto &i : npack.object_list())
	{
		cout<<i->get_class_name()<<"::a="<<((MyObject*)i.get())->a<<", ";
		cout<<i->get_class_name()<<"::b="<<((MyObject*)i.get())->b<<", ";
		cout<<i->get_class_name()<<"::c="<<((MyObject*)i.get())->c<<endl;
	}
}

//normal
void test_serializ1()
{
	cout<<"test_serializ1:"<<endl;
	Jpack pck("aaa", "request");
	MyObject *obj = new MyObject();
	obj->a = 100;
	obj->b = 10.11;
	obj->c = "hello";
	pck.add_object(Jpack::object_ptr_t(obj));
	MyObject *obj2 = new MyObject();
	obj2->a = 1010;
	obj2->b = 101.11;
	obj2->c = "hello,world";
	pck.add_object(Jpack::object_ptr_t(obj2));


	Jpack::serial_t s;
	size_t len;
	const char *buf = s(pck, &len);
	cout<<"result("<<len<<"):";
	cout<<"head:"<<*(int*)buf<<"len:"<<*(int*)(buf+4)<<"payload:"<<buf+8<<endl;
}

/*//no source
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
*/

//unserial
void test_unserialize_1()
{
	cout<<"test_unserialize1:"<<endl;
	string payload="{\"action\":\"trans_msg\",\"source\":\"msgview\",\"target\":\"msg\",\"params\":{\"param0\":{\
	                 \"className\":\"MyObject\", \"a\":\"64000000\",\"b\":\"8fc22141\",\"c\":\"hello\"}}}";
	
	cout<<payload<<endl;
	char buf[1024]={0};
	buf[0] = (char)0xff;
	buf[1] = (char)0xff;
	buf[2] = (char)0xff;
	buf[3] = (char)0xff;

	size_t len =  payload.size();
	memcpy(buf+4, &len, 4);
	memcpy(buf+8, payload.c_str(), len);

	Jpack::unserial_t us(1024);
	Jpack npack;
	int r = us(npack, buf, len+9);
	print_pack(npack);	
}

/*
void test_userialize_2()
{
	string payload="{\"action\":\"trans_msg\",\"source\":\"msgview\",\"target\":\"msg\",\"params\":{\"param0\":[{\"name\":\"msg\",\"value\":\"hello\"},{\"name\":\"from\",\"value\":\"1\"},{\"name\":\"to\",\"value\":\"2\"}]}}";
	
	char buf[1024]={0};
	buf[0] = (char)0xff;
	buf[1] = (char)0xff;
	buf[2] = (char)0xff;
	buf[3] = (char)0xff;

	size_t len =  payload.size();
	memcpy(buf+4, &len, 4);
	memcpy(buf+8, payload.c_str(), len);
	Jpack::unserial_t us(1024);
	Jpack npack;
	int r = us(npack, buf, len+8);
	cout << "result(" << r << "):" << endl;
	print_pack(npack);
}


void test_userialize_3()
{
	ifstream fs("log.txt");

	string payload;
	fs>>payload;
	
	char buf[1024+8]={0};
	buf[0] = (char)0xff;
	buf[1] = (char)0xff;
	buf[2] = (char)0xff;
	buf[3] = (char)0xff;

	Jpack npack;
	Jpack::unserial_t us(1024*1024);
	size_t len =  payload.size();
	memcpy(buf+4, &len, 4);
	int r = us(npack, buf, 8);
	cout<<"result("<<r<<"):"<<endl;

	size_t m = len/1024;
	size_t n = len%1024;
	for(size_t i=0; i<m; i++)
	{
		memcpy(buf, payload.c_str()+i*1024, 1024);
		int r = us(npack, buf, 1024);
		cout<<"result("<<r<<"):"<<endl;
	}

	memcpy(buf, payload.c_str()+m*1024, n);
	r = us(npack, buf, n);
	cout<<"result("<<r<<"):"<<endl;

	print_pack(npack);
}



void test_userialize_4()
{
	cout<<"test_userialize_4:"<<endl;

	string payload="{\"action\":\"trans_msg\",\"source\":\"msgview\",\"target\":\"msg\",\"params\":{\"param0\":[{\"name\":\"msg\",\"value\":\"hello\"},{\"name\":\"from\",\"value\":\"1\"},{\"name\":\"to\",\"value\":\"2\"}]}}";
	
	char buf[1024]={0};
	buf[0] = (char)0xff;
	buf[1] = (char)0xff;
	buf[2] = (char)0xff;
	buf[3] = (char)0xff;

	size_t len =  payload.size();
	memcpy(buf+4, &len, 4);
	memcpy(buf+8, payload.c_str(), len);
	buf[len+8+0] = (char)0xff;
	buf[len+8+1] = (char)0xff;
	buf[len+8+2] = (char)0xff;
	buf[len+8+3] = (char)0xff;
	memcpy(buf+4+len+8, &len, 4);
	memcpy(buf+8+len+8, payload.c_str(), len);
	Jpack::unserial_t us(1024);
	Jpack npack;
	int r = us(npack, buf, len+8+len+8);
	cout<<"result("<<r<<"):"<<endl;
	cout<<"action:"<<npack.action()<<endl;
	cout<<"source:"<<npack.source()<<endl;

	r = us(npack, nullptr, 0);
	cout<<"result("<<r<<"):"<<endl;
	cout<<"action:"<<npack.action()<<endl;
	cout<<"source:"<<npack.source()<<endl;

}

void test_userialize_plus_1()
{
	string payload = "{\"action\":\"trans_msg\",\"source\":\"msgview\",\"target\":\"msg\",\"params\":";
	payload += "{\"param0\":[{\"name\":\"msg\",\"value\":\"hello\"},{\"name\":\"from\",\"value\":\"1\"},{\"name\":\"to\",\"value\":\"2\"}]";
	payload += ",\"param1\":[{\"name\":\"msg\",\"value\":\"hello2\"},{\"name\":\"from\",\"value\":\"1\"},{\"name\":\"to\",\"value\":\"2\"}]}}";

	string payload2 = "{\"action\":\"trans_msg\",\"source\":\"msgview\",\"target\":\"msg\",\"params\":";
	payload2 += "{\"param0\":[{\"name\":\"msg\",\"value\":\",world\"},{\"name\":\"from\",\"value\":\"\"},{\"name\":\"to\",\"value\":\"\"}]";
	payload2 += ",\"param1\":[{\"name\":\"msg\",\"value\":\",world2\"},{\"name\":\"from\",\"value\":\"\"},{\"name\":\"to\",\"value\":\"\"}]}}";

	char buf[1024] = { 0 };
	buf[0] = (char)0xff;
	buf[1] = (char)0xff;
	buf[2] = (char)0xff;
	buf[3] = (char)0xff;

	size_t len = payload.size();
	memcpy(buf + 4, &len, 4);
	memcpy(buf + 8, payload.c_str(), len);
	Jpack::unserial_t us(1024);
	Jpack npack1;
	int r = us(npack1, buf, len + 8);

	size_t len2 = payload2.size();
	memcpy(buf + 4, &len2, 4);
	memcpy(buf + 8, payload2.c_str(), len2);
	Jpack npack2;
	r = us(npack2, buf, len + 8);

	npack1 += npack2;
	print_pack(npack1);

}


void test_userialize_plus_2()
{
	string payload = "{\"action\":\"trans_msg\",\"source\":\"msgview\",\"target\":\"msg\",\"params\":";
	payload += "{\"param0\":[{\"name\":\"msg\",\"value\":\"hello\"},{\"name\":\"from\",\"value\":\"1\"},{\"name\":\"to\",\"value\":\"2\"}]";
	payload += ",\"param1\":[{\"name\":\"msg\",\"value\":\"hello2\"},{\"name\":\"from\",\"value\":\"1\"},{\"name\":\"to\",\"value\":\"2\"}]}}";

	string payload2 = "{\"action\":\"trans_msg\",\"source\":\"msgview\",\"target\":\"msg\",\"params\":";
	payload2 += "{\"param0\":[{\"name\":\"msg\",\"value\":\",world\"},{\"name\":\"to\",\"value\":\"\"}]";
	payload2 += ",\"param2\":[{\"name\":\"msg\",\"value\":\",world2\"},{\"name\":\"to\",\"value\":\"\"}]}}";

	char buf[1024] = { 0 };
	buf[0] = (char)0xff;
	buf[1] = (char)0xff;
	buf[2] = (char)0xff;
	buf[3] = (char)0xff;

	size_t len = payload.size();
	memcpy(buf + 4, &len, 4);
	memcpy(buf + 8, payload.c_str(), len);
	Jpack::unserial_t us(1024);
	Jpack npack1;
	int r = us(npack1, buf, len + 8);

	size_t len2 = payload2.size();
	memcpy(buf + 4, &len2, 4);
	memcpy(buf + 8, payload2.c_str(), len2);
	Jpack npack2;
	r = us(npack2, buf, len + 8);

	npack1 += npack2;
	print_pack(npack1);

}
*/

int main()
{
	test_serializ1();
	//test_serializ2();
	test_unserialize_1();
	//test_userialize_2();
	//test_userialize_3();
	//test_userialize_4();
	//test_userialize_plus_1();
	//test_userialize_plus_2();
	return 0;
}

