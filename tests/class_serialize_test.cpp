//class_serialize_pattern_test.cpp

#include "../class_serialize_handler.h"
#include "../class_info.h"

#include <iostream>
#include <json/json.h>

using namespace std;
using namespace arsee;

typedef vector<int> vi_t;

DEF_CLASS_BEGIN(A)
	A()
	{
		REGISTER_ATTR(A, int,    a);
		REGISTER_ATTR(A, float,  b);
		REGISTER_ATTR(A, string, c);
	}

	BUILD_ATTR(int,    a);
	BUILD_ATTR(float,  b);
	BUILD_ATTR(string, c);
DEF_CLASS_END(A);

DEF_CLASS_NAME(A);

DEF_CLASS_BEGIN(Object)
	Object()
	{
		REGISTER_ATTR(Object, int,    a);
		REGISTER_ATTR(Object, float,  b);
		REGISTER_ATTR(Object, string, c);
		REGISTER_ATTR(Object, A, d);
		REGISTER_ATTR(Object, vi_t, e);
		REGISTER_ATTR(Object, shared_ptr<A>, f);
	}

	BUILD_ATTR(int,    a);
	BUILD_ATTR(float,  b);
	BUILD_ATTR(string, c);
	BUILD_ATTR(A, d);
	BUILD_ATTR(vi_t, e);
	BUILD_ATTR(shared_ptr<A>, f);
DEF_CLASS_END(Object);

DEF_CLASS_NAME(Object);

void test_serialize()
{
	Object obj;
	obj.a = 10;
	obj.b = 10.10f;
	obj.c = "1000";
	A a;
	a.a=11;
	a.b=11.10f;
	a.c="1001";
	obj.d = a;
	vi_t vi;
	vi.push_back(1);
	vi.push_back(2);
	vi.push_back(3);
	obj.e = vi;
	A *pa = new A;
	pa->a = 12;
	pa->b = 12.12f;
	pa->c = "2222";
	obj.f = shared_ptr<A>(pa);
	string str = ClassSerializer<Object>()(ss, obj);
	cout<<str<<endl;
}

void test_serialize_int()
{
	int i=10;
	string str = ClassSerializer<int>()(ss, i);
	cout<<str<<endl;
}

void test_unserialize()
{
	Json::Reader rd;
	Json::Value schema;
	string stream ="{\"className\":\"Object\",\"a\":\"0a000000\",\"b\":\"9a992141\"\
		       ,\"c\":\"1000\",\"d\":{\"className\":\"A\",\"a\":\"0b000000\"\
		       ,\"b\":\"9a993141\",\"c\":\"1001\"},\"e\":{\"v\":[\"01000000\"\
		       ,\"02000000\",\"03000000\"]},\"f\":{\"className\":\"A\",\"a\":\"0c000000\"\
		       ,\"b\":\"85eb4141\",\"c\":\"2222\"}}";

	if(!rd.parse(stream, schema, false))
	{
		cout<< "stream format error"<<endl;
		return;
	}
	
	Object* obj;
	obj = ClassUnserialize<Object>()(us, schema);
	if(obj == nullptr)
	{
		cout<<"unserial failed!"<<endl;
		return;
	}
	cout << "a=" << obj->a << ",b=" << obj->b<<",c="<<obj->c 
	     <<",d={a="<<obj->d.a<<",b="<<obj->d.b<<",c="<<obj->d.c<<"}";
	
	cout<<",e=[";
	for( auto i:obj->e)
		cout<<i<<" ";
	cout<<"]"<<endl;

	cout<<",f={"<<obj->f->a<<","<<obj->f->b<<","<<obj->f->c<<"}"<<endl;

	delete obj;
}

void test_unserialize_int()
{
	Json::Reader rd;
	Json::Value schema;
	string stream = "{\"className\":\"i\",\"val\":\"0a000000\"}";
	if(!rd.parse(stream, schema, false))
	{
		cout<< "stream format error"<<endl;
		return;
	}
	
	int* obj;
	obj = ClassUnserialize<int>()(us, schema);
	if(obj == nullptr)
	{
		cout<<"unserial failed!"<<endl;
		return;
	}
	cout << "val=" << *obj<<endl;
}
	
int main()
{
	test_serialize();
	//test_serialize_int();
	test_unserialize();
	//test_unserialize_int();
}
