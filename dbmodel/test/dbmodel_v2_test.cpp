#include <iostream>
#include "../dbmodel_v2.h"

using namespace std;
using namespace arsee;

typedef DbModel<int_attr_t
	, str_attr_t
	, double_attr_t
>
Amodel;

int main()
{
	Amodel a(int_attr_t(1), str_attr_t("empty"), double_attr_t(1.1));

	cout << "a=" << a.get_attr<0>() << endl;
	cout << "b=" << a.get_attr<1>() << endl;

	//if (a.get_attr("c")->isnull_)
	//	cout << "c=null" << endl;
	//else
	//	cout << "c=" << a.get_attr("c") << endl;

	//a.set_attr("a", 10);
	//cout << "a=" << a.get_attr("a") << endl;
	//a.set_attr("b", "hello");
	//cout << "b=" << a.get_attr("b") << endl;
	///a.set_attr("c", 1.11);
	//if (a.get_attr("c")->isnull_)
	//	cout << "c=null" << endl;
	//else
	//{
	//	cout << "c=" << a.get_attr("c") << endl;
	//}

	return 0;
}