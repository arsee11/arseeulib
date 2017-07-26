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

#ifndef IOBJECT_H
#include "iobject.h"
#endif

#ifndef BUFFER_WRAPPER_H
#include "../buffer_wrapper.h"
#endif

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
	typedef Pack<Serializer, UnSerializer> pack_t;
	typedef std::shared_ptr<pack_t> pack_ptr_t;
	typedef std::vector<pack_ptr_t> pack_list_t;
	typedef std::string stream_t;
	typedef std::shared_ptr<IObject> object_ptr_t;
	typedef std::vector<object_ptr_t> object_list_t;
	
	typedef UnSerializer unserial_t;
	typedef Serializer serial_t;

	static const short HeadField=4;
	static const short LenField = 4;	//payload len	
	
public:
	Pack(){}
	
	Pack(const stream_t &act, const stream_t &type)
		:_action(act)
		,_type(type)
	{
	} 
	
	Pack(const stream_t &act, const stream_t &type
		,const stream_t &param_type, const stream_t& param_encoding
	)
		:_action(act)
		,_type(type)
		,_paramt(param_type)
		,_parame(param_encoding)
	{
	} 
	
	
	bool result()const{ return _result; }
	void result(bool val){ _result= val; }

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
	
	stream_t type()const{ return std::move(_type); }
	void type(const stream_t &val ){ _type=val; }
	void type(stream_t &&val ){ type(val); }
	
	stream_t get_continue()const{ return std::move(_continue); }
	void set_continue(const stream_t &val ){ _continue=val; }
	void set_continue(stream_t &&val ){ set_continue(val); }
	
	stream_t param_type()const{ return std::move(_paramt); }
	void param_type(const stream_t &val ){ _paramt=val; }
	void param_type(stream_t &&val ){ param_type(val); }
	
	stream_t param_encoding()const{ return std::move(_parame); }
	void param_encoding(const stream_t &val ){ _parame=val; }
	void param_encoding(stream_t &&val ){ param_encoding(val); }
	
	void add_object(object_ptr_t obj){ _object_list.push_back(obj); }
	object_list_t object_list()const{ return _object_list; }
	
	pack_t& operator+=( pack_t& rhs)
	{
		if( IsMatch(rhs) )
		{
			object_list_t::iterator i = this->object_list().begin();
			object_list_t::const_iterator j = rhs.object_list().begin();
			for(; i!= this->object_list().end(); ++i, ++j)
			{
				*(*i)+=*(*j);
			}
		}
		return (*this);		
	}
	
		
	void Reset()
	{
		_status = false;
		_result= false;
		_action="";
		_target="";
		_source="";
		_type="request";
		_continue="";
		_paramt="text";
		_parame="plain";		
		_object_list.clear();
	}
	
private:
	bool IsMatch(pack_t& other)
	{
		if (this->action() != other.action())
			return false;

		if( this->_object_list.size() != other.object_list().size() )
			return false;
		
		object_list_t::iterator i = this->_object_list.begin();
		object_list_t::const_iterator j = other.object_list().begin();
		for(; i!= _object_list.end(); ++i, ++j)
		{
			if( std::string((*i)->get_class_name()) != std::string((*j)->get_class_name()))
					return false;
		}
		
		return true;
	}
	
private:
	bool _status=false;
	bool _result=false;
	stream_t _action="";
	stream_t _target="";
	stream_t _source="";
	stream_t _type="request";
	stream_t _continue="";
	stream_t _paramt="text";
	stream_t _parame="plain";
	object_list_t _object_list;
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
		:_buf(3*buf_len)
	{
	}

	//@return >0 successed, otherwise failed.
	int operator()(typename pack_t::pack_list_t &pcks, const char* stream, size_t len)
	{
		cout<<stream+8<<endl;
		_buf.put(stream, len);
		while( Judge() )
		{
			char* buf = new char[_payload_len];
			_buf.get(buf, _payload_len);
			stream_t s(buf, _payload_len);
			//stream_t s(_buf.rptr(), _payload_len);
			//_buf.rptr(_payload_len);
			pack_ptr_t pck = pack_ptr_t(new pack_t);	
			if( Parse(pck, s) > 0 )
			{
				pck->status(true);
				pcks.push_back(pck);
			}			
		}

		return pcks.size();
	}

	//parse the raw @stream
	//@return >0 success, otherwise failed.
	virtual int Parse(pack_t &pack, stream_t &stream)=0;
	virtual int Parse(pack_ptr_t &pack, stream_t &stream)=0;

	//check the pack header, derive classes must override this method.
	//@stream the raw stream input,
	//@len len of @stream,
	//@head_len return pack header len,
	//@return if nullptr means the header not found, 
	//otherwise ptr to pack stream except header.
	virtual bool Header(BufferWrapper& buf, size_t *head_len)=0;

private:
	//@return nullptr if judge fail,
	//otherwise ptr to pack stream except header and length fields.
	bool Judge()
	{
		size_t head_len= 0;
		if(Header(_buf, &head_len))
		{
			Payload(&_payload_len);
			if(_payload_len > 0)
			{
				if(_buf.size()>=_payload_len)
					return true;
				
				_buf.rptr( -(long)(head_len+pack_t::LenField) );
			}
			else
				_buf.rptr(-(long)head_len);
			
		}

		return false;
	}

	void Payload(size_t *payload_len)
	{
		if( _buf.size() < pack_t::LenField)
		{
			*payload_len=0;
			return;
		}

		*payload_len = *(int*)_buf.rptr();
		_buf.rptr(pack_t::LenField);
	}

protected:
	BufferWrapper _buf;
	size_t _payload_len = 0;
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
#if defined(_MSC_VER)
		strcpy_s(_buf+_hlen+pack_t::LenField, plen, str.c_str());
#else
		strncpy(_buf+_hlen+pack_t::LenField, str.c_str(), plen);
#endif
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

extern bool Head0xff(BufferWrapper& buf, size_t *head_len);

NAMESP_END

#endif /*PACK_H*/
