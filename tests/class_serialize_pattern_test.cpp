//class_serialize_pattern_test.cpp

#include "../class_serialize_pattern.h"

#include <iostream>

using namespace std;
using namespace arsee;


DEF_CLASS_BEGIN(Object)
	Object()
	{
		REGISTER_ATTR(Object, int,    a);
		REGISTER_ATTR(Object, float,  b);
		REGISTER_ATTR(Object, string, c);
	}

	BUILD_ATTR(int,    a);
	BUILD_ATTR(float,  b);
	BUILD_ATTR(string, c);
DEF_CLASS_END(Object)

void test_serialize()
{
	Object obj;
	obj.a = 10;
	obj.b = 10.10f;
	obj.c = "1000";
	string str = serialize(obj);
	cout<<str<<endl;
}

void test_unserialize()
{
	typedef std::map<string, string> map_t;
	map_t m;
	m["a"] = "abcdef01";
	m["b"] = "9a992141";
	m["c"] = "9a992141";
	string name = "Object";
	Object* obj = unserialize<Object>(name, m);
	cout << "a=" << obj->a << ",b=" << obj->b<<",c="<<obj->c << endl;
}

int main()
{
	test_serialize();
	test_unserialize();
}
