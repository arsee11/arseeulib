//executor_test.cpp


#include "../executee.h"

#include <iostream>
#include <chrono>

using namespace std;
using namespace arsee;

class ExecuteeImpl:public Executee_st<ExecuteeImpl>
{
public:
	ExecuteeImpl(ExeScope* exc)
		:Executee(exc)
	{}

	~ExecuteeImpl(){ cout<<__FUNCTION__<<endl;}

	void do_(int a){
		post(&ExecuteeImpl::todo, a);
	}

	std::string n="hello";
private:
	void todo_2(){}
	void todo(int a){
		std::this_thread::sleep_for(std::chrono::seconds(1));
		cout<< "todo("<<a<<") in: "<<current_scope()<< ", exc scope:"<<scope()<<" "<<this->n<<endl;
		post(&ExecuteeImpl::todo, a);
		
	}

};


int main()
{
	ExeScope exc;
	cout<<"myid:"<<exc.current_scope()<<endl;
		ExecuteeImpl ext(&exc);
		ext.do_(123);
		//ext.setScope(nullptr);
	char ch;
	cin>>ch;
	
	return 0;
}
