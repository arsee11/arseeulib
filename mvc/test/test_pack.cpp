//test_pack.cpp

#include "../pack.h"
#include<iostream>



using namespace arsee;
using namespace std;

class simpleS;

class simple:
	public UnSerializerAbstr<simple, simpleS>
{
public:
	simple(size_t len)
		:UnSerializerAbstr(len)
	{}

	int Parse(pack_t &pack, const char *stream) override
	{
		pack.Action(pack_t::stream_t(stream));
	}

	const char* Header(const char* stream, size_t len, size_t *head_len)override
	{
		if(stream[0] == (char)0xFF && stream[1] == (char)0xFF)
		{
			*head_len = 2;
			return stream + 2;
		}

		return nullptr;
	}
	
};

class simpleS:
	public SerializerAbstr
{
public:
	const char* operator()(const pack_t &pck, size_t &len)override
	{
	}
};

void test_unserializer()
{

	cout<<"test_unserializer:"<<endl;
	typedef Pack<simple, simpleS> spack_t;
	spack_t::serial_t us(15);
	spack_t p;
	char buf[10] = {0xff, 0xff, 'h','e','l','l','o','.','.','.'};
	int r = us(p, buf, 10);
	cout<<"result:"<<p.Action()<<endl;
}

void test_unserializer2()
{
	cout<<"test_unserializer2:"<<endl;
	typedef Pack<simple, simpleS> spack_t;
	spack_t::serial_t us(10);
	spack_t p;
	char buf[10] = {0xff, 0xff, 'h','e','l','l','o','.','.','.'};
	int r = us(p, buf, 10);
	cout<<"result:"<<p.Action()<<endl;
}

void test_unserializer3()
{
	cout<<"test_unserializer3:"<<endl;
	typedef Pack<simple, simpleS> spack_t;
	spack_t::serial_t us(10);
	spack_t p;
	char buf[13] = {0xff, 0xff};
	strcpy(buf+2, "1234567890");
	int r = us(p, buf, 12);
	cout<<"result:"<<p.Action()<<endl;
}

void test_unserializer4()
{
	cout<<"test_unserializer4:"<<endl;
	typedef Pack<simple, simpleS> spack_t;
	spack_t::serial_t us(10);
	spack_t p;
	char buf[15] = {0xff, 0xff};
	strcpy(buf+2, "1234567890123");
	int r = us(p, buf, 12);
	cout<<"result:"<<p.Action()<<endl;
}

int main()
{
	test_unserializer();
	test_unserializer2();
	test_unserializer3();
	test_unserializer4();
	return 0;
}
