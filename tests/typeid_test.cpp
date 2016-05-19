//typeid_test.cpp


#include <vector>
#include <list>
#include <iostream>
#include <typeinfo>
#include "../rqh/utility.h"


using namespace std;

template<class T, template<class, class> class V>
struct IsVector
{
	typedef char ONE;
	typedef struct{char c[2];} TWO;
	T t;
	
	template<class U>
	static ONE is_vector(V<U, allocator<U> > u);
	template<class U>
	static TWO is_vector(U u);
	enum{ isVector=sizeof(IsVector<T, V>::is_vector(t))};
};

struct Handle
{
	void operator()()
	{
		cout<<"this is a vecotr"<<endl;
	}
};
struct HandleNot
{
	void operator()()
	{
		cout<<"this is not  a vecotr"<<endl;
	}
};

int main()
{
	typedef std::vector<int> v1_t;
	cout<<"is vector:"<<IsVector<v1_t, vector>::isVector<<endl;
	cout<<"is vector:"<<IsVector<v1_t, list>::isVector<<endl;
	typedef std::vector<int> v3_t;
	typedef std::vector<float> v2_t;

	typedef IfElseThen<IsVector<v1_t, vector>::isVector==1,
			Handle, HandleNot>::value handle_t;
	
	handle_t()();

	cout<<"v1_t:"<<typeid(v1_t).name()<<",v2_t:"<<typeid(v2_t).name()<<",v3_t:"<<typeid(v3_t).name()<<endl;

}

