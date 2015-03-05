#include <iostream>
#include "../dbmodel.h"

using namespace std;
using namespace arsee;

class Amodel:public DbModel
{
public:
	Amodel()
	{
		_attributes["a"] = int_attr_t::New(0);
		_attributes["b"] = str_attr_t::New("empty");
		_attributes["c"] = double_null_attr_t::New();
	}

	~Amodel()
	{
		cout << "~Amodel()" << endl;
	}

	string get_table()const{ return "table_a"; }

};

int main()
{
	Amodel a;
	cout <<"a="<< a.get_attr<int>("a") << endl;
	cout <<"b="<< a.get_attr<string>("b") << endl;

	if (a.get_attr("c")->isnull_)
		cout <<"c=null" << endl;
	else
		cout <<"c="<< a.get_attr<double>("c") << endl;

	a.set_attr("a", 10);
	cout <<"a="<< a.get_attr<int>("a") << endl;
	a.set_attr("b", "hello");
	cout <<"b="<< a.get_attr<string>("b")<<endl;
	a.set_attr("c", 1.11);
	if (a.get_attr("c")->isnull_)
		cout << "c=null" << endl;
	else
	{
		cout << "c=" << a.get_attr<double>("c") << endl;
	}

	return 0;
}