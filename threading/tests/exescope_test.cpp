//executor_test.cpp


#include "../exescope.h"
#include <iostream>

using namespace std;
using namespace arsee;

class Executee
{
public:
	Executee(ExeScope* exc)
		:_exc(exc)
	{}

	void do(int a){
		assert(_exc);
		_exc->post( std::bind(&Executee::todo, this, _1), a);
	}

private:
	void todo(int a){
		cout<< "todo in: "<< _exc->current_scope()<< "exc scope:"<<_exc->scope()<<endl;
	}

	Executor* _exc;
};


int main()
{
	ExeScope exc;
	Executee ext(&exc);
	
	return 0;
}
