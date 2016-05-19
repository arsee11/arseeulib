//serialize_vector_test.cpp


#include "../class_serialize_handler.h"
#include "../class_info.h"

#include <iostream>
#include <vector>
#include <json/json.h>

using namespace std;
using namespace arsee;

void test_vector_int()
{
	cout<<"test_vector_int"<<endl;
	typedef vector<int> v_t;
	v_t v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	
	string stream ="{"+ SerializeVector(v, v.size())+"}";
	cout<<stream<<endl;
}

void test_usvector_int()
{
	cout<<"test_usvector_int"<<endl;
	Json::Reader rd;
	Json::Value schema;
	string stream ="{ \"v\":[\"01000000\",\"02000000\",\"03000000\"]}";
	if(!rd.parse(stream, schema, false))
	{
		cout<< "stream format error"<<endl;
		return;
	}

	typedef vector<int> v_t;
	std::string attr_name = "vec";
	std::string attr_type = "vector";
	v_t *v = (v_t*)Us<v_t>()(schema, attr_name, attr_type, 0); 

	v_t::iterator i = v->begin();
	for(;i != v->end(); ++i)
		cout<<*i<<endl;

	delete v;
}

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

void test_vector_A()
{
	cout<<"test_vector_A"<<endl;
	typedef vector<A> v_t;
	v_t v;
	A a1;
	a1.a=100;
	a1.b=10.1;
	a1.c="abc";
	v.push_back(a1);
	A a2;
	a2.a=100;
	a2.b=10.1;
	a2.c="abc";
	v.push_back(a2);
	A a3;
	a3.a=100;
	a3.b=10.1;
	a3.c="abc";
	v.push_back(a3);
	A a4;
	a4.a=100;
	a4.b=10.1;
	a4.c="abc";
	v.push_back(a4);
	
	string stream ="{"+ SerializeVector(v, v.size())+"}";
	cout<<stream<<endl;
}

void test_usvector_A()
{
	cout<<"test_usvector_A"<<endl;
	Json::Reader rd;
	Json::Value schema;
	string stream ="{\"v\":\
	[{\"className\":\"A\", \"a\":\"01000000\",\"b\":\"02002000\",\"c\":\"abc\"}\
	,{\"className\":\"A\", \"a\":\"01000000\",\"b\":\"02002000\",\"c\":\"abc\"}\
	,{\"className\":\"A\", \"a\":\"01000000\",\"b\":\"02002000\",\"c\":\"abc\"}]}";
	if(!rd.parse(stream, schema, false))
	{
		cout<< "stream format error"<<endl;
		return;
	}

	typedef vector<A> v_t;
	std::string attr_name = "vec";
	std::string attr_type = "vector";
	v_t *v = (v_t*)Us<v_t>()(schema, attr_name, attr_type, 0); 

	v_t::iterator i = v->begin();
	for(;i != v->end(); ++i)
		cout<<i->a<<","<<i->b<<","<<i->c<<endl;
}

int main()
{
	test_vector_int();
	test_usvector_int();
	test_vector_A();
	test_usvector_A();
	return 0;
}
