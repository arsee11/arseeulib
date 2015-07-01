//test_pack.cpp

#include "../pack.h"
#include<iostream>



using namespace arsee;
using namespace std;

class simpleS;

class simple:
	public UnSerializerAbstr<simpleS, simple>
{
public:
	simple()
		:UnSerializerAbstr<simpleS, simple>(100)
	{}

	int Parse(pack_t &pack, stream_t &stream) override 
	{ 
		pack.action(pack_t::stream_t(stream));
		return stream.size();
	}

	const char* Header(const char* stream, size_t len, size_t *head_len)override
	{
		for(size_t i=0; i<len-1; ++i)
		{
			if(stream[i] == (char)0xFF && stream[i+1] == (char)0xFF)
			{
				*head_len = 2;
				return stream + (i+2);
			}
		}

		return nullptr;
	}
	
};


//less than pack
void test_unserializer()
{

	cout<<"test_unserializer:"<<endl;
	typedef Pack<simpleS, simple> spack_t;
	spack_t::unserial_t us;
	spack_t p;
	char buf[20] = {0xff, 0xff};
	long len = 10;
	memcpy(buf+2, &len, sizeof(long)); 
	strcpy(buf+2+sizeof(long), "1234567890");
	int r = us(p, buf, 15);
	cout<<"result("<<r<<"):"<<p.action()<<endl;
}

//completed pack
void test_unserializer2()
{
	cout<<"test_unserializer2:"<<endl;
	typedef Pack<simpleS, simple> spack_t;
	spack_t::unserial_t us;
	spack_t p;
	char buf[20] = {0xff, 0xff};
	long len = 10;
	memcpy(buf+2, &len, sizeof(long)); 
	strcpy(buf+2+sizeof(long), "1234567890");
	int r = us(p, buf, 20);
	cout<<"result("<<r<<"):"<<p.action()<<endl;
}

//more than pack 
void test_unserializer3()
{
	cout<<"test_unserializer3:"<<endl;
	typedef Pack<simpleS, simple> spack_t;
	spack_t::unserial_t us;
	spack_t p;
	char buf[25] = {0xff, 0xff};
	long len = 10;
	memcpy(buf+2, &len, sizeof(long)); 
	strcpy(buf+2+sizeof(long), "12345678901234");
	int r = us(p, buf, 25);
	cout<<"result("<<r<<"):"<<p.action()<<endl;
}

//header fail
void test_unserializer5()
{
	cout<<"test_unserializer5:"<<endl;
	typedef Pack<simpleS, simple> spack_t;
	spack_t::unserial_t us;
	spack_t p;
	char buf[15] = {0xaa, 0xff};
	strcpy(buf+2, "1234567890123");
	int r = us(p, buf, 12);
	cout<<"result("<<r<<"):"<<p.action()<<endl;
}

//size > 0 two pack
void test_unserializer6()
{
	cout<<"test_unserializer6:"<<endl;
	typedef Pack<simpleS, simple> spack_t;
	spack_t::unserial_t us;
	spack_t p;
	char buf[25] = {0xff, 0xff};
	long len = 10;
	memcpy(buf+2, &len, sizeof(long)); 
	strcpy(buf+2+sizeof(long), "12345678901234");
	int r = us(p, buf, 25);
	cout<<"result("<<r<<"):"<<p.action()<<endl;
	r = us(p, buf, 25);
	cout<<"result("<<r<<"):"<<p.action()<<endl;
	r = us(p, buf+2, 12);
	cout<<"result("<<r<<"):"<<p.action()<<endl;
}

//one byte 
void test_unserializer7()
{
	cout<<"test_unserializer7:"<<endl;
	typedef Pack<simpleS, simple> spack_t;
	spack_t::unserial_t us;
	spack_t p;
	char byte = (char)0xff;
	int r = us(p, &byte, 1);
	cout<<"result1("<<r<<"):"<<p.action()<<endl;

	r = us(p, &byte, 1);
	cout<<"result2("<<r<<"):"<<p.action()<<endl;

	long len = 10;
	char *buf = (char*)&len;
	r = us(p, buf, 1);
	cout<<"result3("<<r<<"):"<<p.action()<<endl;
	r = us(p, buf+1, 1);
	cout<<"result4("<<r<<"):"<<p.action()<<endl;
	r = us(p, buf+2, 1);
	cout<<"result5("<<r<<"):"<<p.action()<<endl;
	r = us(p, buf+3, 1);
	cout<<"result6("<<r<<"):"<<p.action()<<endl;
	r = us(p, buf+4, 1);
	cout<<"result7("<<r<<"):"<<p.action()<<endl;
	r = us(p, buf+5, 1);
	cout<<"result8("<<r<<"):"<<p.action()<<endl;
	r = us(p, buf+6, 1);
	cout<<"result9("<<r<<"):"<<p.action()<<endl;
	r = us(p, buf+7, 1);

	char *payload ="12345678901234";
	r = us(p, payload, 14);
	cout<<"result10("<<r<<"):"<<p.action()<<":"<<p.action().size()<<endl;
}

int main()
{
	//test_unserializer();
	//test_unserializer2();
	//test_unserializer3();
	//test_unserializer5();
	//test_unserializer6();
	test_unserializer7();
	return 0;
}
