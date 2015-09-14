//class_serialize_pattern_test.cpp

#include "../class_serialize_pattern.h"

#include <iostream>

using namespace std;
using namespace arsee;


extern char const class_Object[]="Object";

struct Object
{
	Object()
	{
		REGISTER_ATTR(Object, int, a);
		REGISTER_ATTR(Object, float, b);
		REGISTER_ATTR(Object, string, c);
	}

	BUILD_ATTR(int,   a);
	BUILD_ATTR(float, b);
	BUILD_ATTR(string, c);

	ClassInfo<Object,class_Object> class_info;
};

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
	m["a"] = "0a000000";
	m["b"] = "9a992141";
	string name = "Object";
	Object* obj = unserialize<Object>(name, m);
	cout << "a:" << obj->a << ",b:" << obj->b << endl;
}

int main()
{
	Register<Object, class_Object> r = ClassInfo<Object, class_Object>::r;
 
	test_serialize();
	test_unserialize();
}