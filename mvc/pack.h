
//file: pack.h

#ifndef PACK_H
#define PACK_H

#ifndef GLOBALDEF_H
#include "globaldef.h"
#endif

#include <string>
#include <map>

NAMESP_BEGIN

///////////////////////////////////////////////////////////////////////////
template<class Serializer, class UnSerializer>
class Pack
{
public:
	typedef Pack pack_t;
	typedef std::map<std::string, std::string> params_pack_t;
	typedef std::string stream_t;
	
	Pack(){}
	
	
	typedef UnSerializer unserializer_t;
	typedef Serializer   serializer_t;
	
public:
	Pack(std::string src, std::string trgt, std::string act, std::string param)
		:_source(src)
		,_target(trgt)
		,_action(act)
	{
		_params["msg"] = param;
	}
	
	Pack(std::string src, std::string trgt, std::string act, params_pack_t &params)
		:_source(src)
		,_target(trgt)
		,_action(act)
		,_params(params)
	{
	}
	
	bool Status(){ return _status; }
	void Status(bool val){ _status = val; }

	std::string Action()const{ return std::move(_action); }
	std::string Target()const{ return std::move(_target); }
	std::string Source()const{ return std::move(_source); }
	params_pack_t Params()const{return std::move(_params);} 

private:
	bool _status =false;
	std::string _action="";
	std::string _target="";
	std::string _source="";
	params_pack_t _params;
};

template<class Pack>
class UnSerializerAbtr
{
public:
	UnSerializerAbtr(size_t len)
		:_len(len)
	{
		_buf = new char[3*_len];
		memset(_buf, 0, 3*_len];
	}

	~UnSerializerAbtr()
	{
		delete[] _buf;
		_buf = nullptr;
	}

	template<class Pack>
	int operator()(const Pack &pck, const char* stream)
	{

		if( Check(stream, _buf, pack )
		{
			//get pack
			
			return Parse(pck, stream); 
		}

		return false;
	}

	virtual int Parse(const Pack &pack, const char *stream);
	virtual char* Header(const char* stream, size_t len);


private:
	bool Check(const char *stream, size_t len)
	{
		if(_size == 0)
		{
			stream = Header(stream, len);
			if(stream != nullptr)

		}
	}

private:
	char *_buf = nullptr;
	size_t _len = 1;
	size_t _size;

};

NAMESP_END

#endif /*PACK_H*/
