
//file: pack.h

#ifndef PACK_H
#define PACK_H

#ifndef GLOBALDEF_H
#include "globaldef.h"
#endif

#include <string>
#include <string.h>
#include <map>

NAMESP_BEGIN

///////////////////////////////////////////////////////////////////////////

template<
	class Serializer,
	class UnSerializer
>
class Pack
{
public:
	typedef Pack pack_t;
	typedef std::string stream_t;
	typedef std::map<stream_t, stream_t> params_pack_t;
	
	typedef UnSerializer unserial_t;
	typedef Serializer serial_t;

	Pack(){}
	
	
	
public:
	Pack(stream_t src, stream_t trgt, stream_t act, stream_t param)
		:_source(src)
		,_target(trgt)
		,_action(act)
	{
		_params["msg"] = param;
	}
	
	Pack(stream_t src, stream_t trgt, stream_t act, params_pack_t &params)
		:_source(src)
		,_target(trgt)
		,_action(act)
		,_params(params)
	{
	}
	
	bool Status()const{ return _status; }
	void Status(bool val){ _status = val; }

	stream_t Action()const{ return std::move(_action); }
	void Action(stream_t &&val ){ _action=val; }
	
	stream_t Target()const{ return std::move(_target); }
	void Target(stream_t &&val ){ _target=val; }

	stream_t Source()const{ return std::move(_source); }
	void Source(stream_t &&val ){ _source=val; }

	params_pack_t Params()const{return std::move(_params);} 
	void Param(stream_t &&name, stream_t &&val){ _params[name] = val; }

private:
	bool _status =false;
	stream_t _action="";
	stream_t _target="";
	stream_t _source="";
	params_pack_t _params;
};


class SerializerAbstr;

//template<
//	class Serializer,
//	class UnSerializer
//>
//class PackImpl:public Pack
//{
//};

template<class DeriveSerial, class DeriveUnSerial>
class UnSerializerAbstr
{
protected:
	typedef Pack<DeriveSerial, DeriveUnSerial> pack_t; 

public:
	UnSerializerAbstr() = delete;

	UnSerializerAbstr(size_t len)
		:_pack_len(len)
	{
		_buf = new char[3*_pack_len];
		memset(_buf, 0, 3*_pack_len);
	}

	~UnSerializerAbstr()
	{
		delete[] _buf;
		_buf = nullptr;
	}

	
	int operator()(pack_t &pck, const char* stream, size_t len)
	{
		const char *pstream = nullptr;	
		if( ( pstream = Judge(stream, len) ) != nullptr )
		{
			return Parse(pck, pstream); 
		}

		return -1;
	}

	//parse the raw @stream
	//@return >0 success, otherwise failed.
	virtual int Parse(pack_t &pack, const char *stream)=0;

	//check the pack header, derive classes must override this method.
	//@stream the raw stream input,
	//@len len of @stream,
	//@head_len return pack header len,
	//@return if nullptr means the header not found, 
	//others pack header position pointer.
	virtual const char* Header(const char* stream, size_t len, size_t *head_len)=0;


private:
	const char* Judge(const char *stream, size_t len)
	{
		//make sure less memory copy
		const char *pbuf = nullptr;
		if(_size == 0)
		{
			size_t head_len= 0;
			pbuf = Header(stream, len, &head_len);
			if(pbuf != nullptr)
			{
				if(_pack_len < len-head_len)
				{
					size_t right_size = len - head_len - _pack_len;
					memcpy(_buf, pbuf+_pack_len, right_size);
					_size = right_size;
				}
				else if(_pack_len > len-head_len) 
				{
					memcpy(_buf, pbuf, len);
					_size = len;
					pbuf = nullptr;
				}
			}
		}
		else
		{
			size_t head_len= 0;
			memcpy(_buf+_size, stream, len);
			_size += len;
			pbuf = Header(_buf, _size, &head_len);
			if(pbuf != nullptr)
			{
				_size -= (_pack_len+head_len);
			}
		}

		return pbuf;
	}

private:
	char *_buf = nullptr;
	size_t _pack_len = 1;
	size_t _size = 0;

};


class SerializerAbstr
{
protected:
	typedef Pack<SerializerAbstr, SerializerAbstr> pack_t; 

public:
	virtual const char* operator()(const pack_t &pck, size_t &len)=0;

};

NAMESP_END

#endif /*PACK_H*/
