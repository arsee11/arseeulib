//pack.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify:	
//2014-12-15
//record 
//****************************

#ifndef PACK_H
#define PACK_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef _STRING_
#include <string>
#endif

#ifndef _INC_STRING
#include <string.h>
#endif

#include <map>
#include <vector>
#include <memory>
#include <iostream>
using namespace std;
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
	typedef std::shared_ptr<pack_t> pack_ptr_t;
	typedef std::vector<pack_ptr_t> pack_list_t;
	typedef std::string stream_t;
	typedef std::map<stream_t, stream_t> params_pack_t;
	
	typedef UnSerializer unserial_t;
	typedef Serializer serial_t;


	static const short HeadField=4;
	static const short LenField = 4;

		
	
public:
	Pack(){}
	Pack(const stream_t &src, const stream_t &trgt, const stream_t &act, const stream_t &param)
		:_source(src)
		,_target(trgt)
		,_action(act)
		,_status(true)
	{
		_params["msg"] = param;
	}
	
	Pack(const stream_t &src, const stream_t &trgt, const stream_t &act, const params_pack_t &params)
		:_source(src)
		,_target(trgt)
		,_action(act)
		,_params(params)
		,_status(true)
	{
	}

	Pack(const stream_t &src, stream_t &&trgt, stream_t &&act, const params_pack_t &params)
		:Pack(src, trgt, act, params)
	{
	}
	
	
	
	Pack(stream_t &&src, stream_t &&trgt, stream_t &&act, params_pack_t &&params)
		:Pack(src, trgt, act, params)
	{
	}
	
	Pack(const stream_t &src, const stream_t &trgt, const stream_t &act)
		:_source(src)
		,_target(trgt)
		,_action(act)
	{
	} 
	
	Pack(stream_t &&src, stream_t &&trgt, stream_t &&act)
		:Pack(src, trgt, act)
	{
	}
	
	Pack(const stream_t &src, stream_t &&trgt, stream_t &&act)
		:Pack(src, trgt, act)
	{
	}

	Pack(stream_t &&src, const stream_t &trgt, stream_t &&act)
		:Pack(src, trgt, act)
	{
	}

	Pack(stream_t &&src, stream_t &&trgt, const stream_t &act)
		:Pack(src, trgt, act)
	{
	}

	Pack(stream_t &&src, const stream_t &act)
		:Pack(src, act)
	{
	}
	
	Pack(stream_t &&src, stream_t &&act)
		:Pack(src, act)
	{
	}
	
	Pack(const stream_t &src, stream_t &&act)
		:Pack(src, act)
	{
	}
	
	Pack(const stream_t &src, const stream_t &act)
		:_source(src)
		,_action(act)
	{
	}

	bool status()const{ return _status; }
	void status(bool val){ _status = val; }

	stream_t action()const{ return std::move(_action); }
	void action(const stream_t &val ){ _action=val; }
	void action(stream_t &&val ){ _action=val; }
	
	stream_t target()const{ return std::move(_target); }
	void target(stream_t &&val ){ _target=val; }

	stream_t source()const{ return std::move(_source); }
	void source(const stream_t &val ){ _source=val; }
	void source(stream_t &&val ){ source(val); }

	params_pack_t params()const{return std::move(_params);} 
	void param(stream_t &&name, stream_t &&val){ _params[name] = val; }
	void param(const stream_t &name, const stream_t &val){ _params[name] = val; }
	void param(stream_t &&name, const stream_t &val){ _params[name] = val; }
	void param(const stream_t &name, stream_t &&val){ _params[name] = val; }
	void param(const char *name, stream_t &&val){ _params[name] = val; }

	void Reset()
	{
		_status = false;
		_action="";
		_target="";
		_source="";
		_params.clear();
	}

private:
	bool _status =false;
	stream_t _action="";
	stream_t _target="";
	stream_t _source="";
	params_pack_t _params;
};


template<class DeriveSerial, class DeriveUnSerial>
class UnSerializerAbstr
{
protected:
	typedef Pack<DeriveSerial, DeriveUnSerial> pack_t; 
	typedef typename pack_t::stream_t stream_t;
	typedef typename pack_t::pack_ptr_t pack_ptr_t;
	
public:
	UnSerializerAbstr() = delete;

	UnSerializerAbstr(size_t buf_len)
		:_buf_len(buf_len)
	{
		cout<<"UnSerializerAbstr()"<<endl;
		_buf = new char[3*_buf_len];
		memset(_buf, 0, 3*_buf_len);
	}

	~UnSerializerAbstr()
	{
		cout<<"~UnSerializerAbstr()"<<endl;
		delete[] _buf;
		_buf = nullptr;
	}

	//@return >0 successed, otherwise failed.
	int operator()(pack_t &pck, const char* stream, size_t len)
	{
		pck.Reset();
		const char *pstream = nullptr;	
		if( ( pstream = Judge(stream, len) ) != nullptr )
		{
			stream_t s(pstream, _payload_len);
			return Parse(pck, s); 
		}

		return -1;
	}

	//parse the raw @stream
	//@return >0 success, otherwise failed.
	virtual int Parse(pack_t &pack, stream_t &stream)=0;

	//check the pack header, derive classes must override this method.
	//@stream the raw stream input,
	//@len len of @stream,
	//@head_len return pack header len,
	//@return if nullptr means the header not found, 
	//otherwise ptr to pack stream except header.
	virtual const char* Header(const char* stream, size_t len, size_t *head_len)=0;

private:
	//@return nullptr judge fail,
	//otherwise ptr to pack stream except header.
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
				pbuf = Payload(pbuf, len-head_len, &_payload_len);
				//head field, paylaod_len field.
				size_t nleft = len-head_len-pack_t::LenField;
				//more than pack
				if(_payload_len < nleft)
				{
					size_t right_size = nleft - _payload_len;
					memcpy(_buf, pbuf+_payload_len, right_size);
					_size = right_size;
				}
				//less than pack
				else if(_payload_len > nleft) 
				{
					memcpy(_buf, pbuf, len);
					_size = len;
					pbuf = nullptr;
				}
				//complete pack
			}
			else
			{
				memcpy(_buf, stream, len);
				_size = len;
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
				pbuf = Payload(pbuf, len-head_len, &_payload_len);
				size_t plen = _payload_len + head_len + pack_t::LenField;
				if(_size < plen)
					pbuf = nullptr;
				else
					_size -= plen; 
			}
		}

		return pbuf;
	}

	const char* Payload(const char* stream, size_t len, size_t *payload_len)
	{
		if(len < pack_t::LenField)
		{
			*payload_len=0;
			return stream;
		}

		//sizeof(int)==4
		*payload_len = *(int*)stream;
		return stream + pack_t::LenField; 
	}

protected:
	char *_buf = nullptr;
	size_t _payload_len = 0;
	size_t _buf_len;
	size_t _size = 0;

};


template<class DeriveSerial, class DeriveUnSerial>
class SerializerAbstr
{
public:
	typedef Pack<DeriveSerial, DeriveUnSerial> pack_t; 
	typedef typename pack_t::stream_t stream_t;
	typedef typename pack_t::pack_ptr_t pack_ptr_t;

public:
	virtual ~SerializerAbstr()
	{
		//if(_buf != nullptr )
		//	delete[] _buf;
		//if(_head != nullptr)
		//	delete _head;
	}
	
	//@return new memory must delete it after use
	const char* operator()(const pack_t &pck, size_t *len)
	{
		stream_t str = Resolve(pck);
		return Build(str, len);
	}

	//@return new memory must delete it after use
	const char* operator()(const pack_ptr_t &pck, size_t *len)
	{
		stream_t str = Resolve(pck);
		return Build(str, len);
	}
	
protected:
	virtual size_t Header()=0;
	virtual stream_t Resolve(const pack_t &pck)=0;
	virtual stream_t Resolve(const pack_ptr_t &pck)=0;
	
	const char* Build(stream_t &str, size_t* len)
	{
		_hlen = Header();
		long plen = str.size()+1;//add \0
		_buf = new char[_hlen+pack_t::LenField+plen];
		memcpy(_buf, _head, _hlen);
		memcpy(_buf+_hlen, &plen, pack_t::LenField); 
		//memcpy(_buf+_hlen+pack_t::LenField, str.c_str(), plen);
		strcpy(_buf+_hlen+pack_t::LenField, str.c_str());
		*len = _hlen+pack_t::LenField+plen;
		return _buf;
	}

protected:
	char *_buf=nullptr;
	char *_head=nullptr;
	size_t _hlen=0;
};

inline size_t Head0xff(char *&head)
{
	head = new char[4];
	memset(head, 0xff, 4);
	return 4;
}

extern const char* Head0xff(const char *stream, size_t len, size_t *head_len);

NAMESP_END

#endif /*PACK_H*/
