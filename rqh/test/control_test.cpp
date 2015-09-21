//control_test.cpp
#include "../jpack.h"
#include<iostream>
#include<fstream>
#include <string>

#if defined(_MSC_VER)
#pragma comment(lib, "../../lib/jsonlibd")
#endif

#include "../../class_info.h"
#include "../object_logic.h"
#include "../control.h"
#include "../iobject.h"

using namespace arsee;
using namespace std;

DEF_CLASS_INHERIT_BEGIN(MyObject, IObject)
	MyObject()
	{
		REGISTER_ATTR(MyObject, int,    a);
		REGISTER_ATTR(MyObject, float,  b);
		REGISTER_ATTR(MyObject, string, c);
	}
	const char* get_class_name(){ return class_info.get_class_name();}
	ClassInfoBase* get_class_info(){ return &class_info; }
	BUILD_ATTR(int,    a);
	BUILD_ATTR(float,  b);
	BUILD_ATTR(string, c);
DEF_CLASS_END(MyObject)

class Logic : public ObjectLogic<Jpack>
{
public: 
	static string name(){ return "trans_msg"; }
	
	IRResponse<Jpack>* Exec(object_list_t& objs) override
	{
		for (auto &i : objs)
		{
			cout<<i->get_class_name()<<"::a="<<((MyObject*)i.get())->a<<", ";
			cout<<i->get_class_name()<<"::b="<<((MyObject*)i.get())->b<<", ";
			cout<<i->get_class_name()<<"::c="<<((MyObject*)i.get())->c<<endl;
		}
	}
};

typedef RControl<Jpack, ObjectLogic<Jpack> > control_t;

void test_execute()
{
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
	Jpack pack;
	us(pack, buf, len+9);
	Logic logic;
	control_t ctrl(string("sourceView"), &logic, nullptr);
	ctrl.Request(pack);
	
}

int main()
{
	test_execute();
	return 0; 
}
