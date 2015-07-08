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
	typedef Pack<Serializer, UnSerializer> pack_t;
	typedef std::shared_ptr<pack_t> pack_ptr_t;
	typedef std::vector<pack_ptr_t> pack_list_t;
	typedef std::string stream_t;
	typedef std::map<stream_t, stream_t> param_item_t;
	typedef std::vector<param_item_t> params_pack_t;
	
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

	void append_param(param_item_t& val){ _params.push_back(val); }
	const params_pack_t& params()const{return _params;} 
	
	//stream_t get_param(size_t i, stream_t &&name)		{ return get_param(i, name); }
	//stream_t get_param(size_t i, const char *name )	{ return get_param(i, stream_t(name)); }
	stream_t get_param(size_t i, const stream_t &name)	
	{ 
		if(_params.size()>=i)
			return _params[i][name]; 
			
		return stream_t();
	}
	
	
	pack_t& operator+=( pack_t& rhs)
	{
		if( IsMatch(rhs) )
		{
			params_pack_t::iterator i = this->_params.begin();
			params_pack_t::const_iterator j = rhs.params().begin();
			for(; i!= _params.end(); ++i, ++j)
			{
				for( auto &m : (*i) )
				{
					m.second += (*j).find(m.first)->second;
				}
			}
		}
		
		return (*this);		
	}
	
		
	void Reset()
	{
		_status = false;
		_action="";
		_target="";
		_source="";
		_type="request";
		_continue="";
		_paramt="text";
		_parame="plain";		
		_params.clear();
	}
private:
	bool IsMatch(pack_t& other)
	{
		if (this->action() != other.action())
			return false;

		if( this->_params.size() != other.params().size() )
			return false;
		
		params_pack_t::iterator i = this->_params.begin();
		params_pack_t::const_iterator j = other.params().begin();
		for(; i!= _params.end(); ++i, ++j)
		{
			for( auto &m : (*i) )
			{
				if((*j).find(m.first) == (*j).end() )
					return false;
			}
		}
		
		return true;
	}
	
private:
	bool _status =false;
	stream_t _action="";
	stream_t _target="";
	stream_t _source="";
	stream_t _type="request";
	stream_t _continue="";
	stream_t _paramt="text";
	stream_t _parame="plain";
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
		:_buf_len(3*buf_len)
	{
		//cout<<"UnSerializerAbstr()"<<endl;
		_buf = new char[_buf_len];
		memset(_buf, 0, _buf_len);
	}

	~UnSerializerAbstr()
	{
		//cout<<"~UnSerializerAbstr()"<<endl;
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
			int r=0;
			if( (r=Parse(pck, s))>0 )
			{
				pck.status(true);
				return r;
			}
			
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
	//@return nullptr if judge fail,
	//otherwise ptr to pack stream except header and length fields.
	const char* Judge(const char *stream, size_t len)
	{
		//make sure less memory copy
		const char *pbuf = nullptr;
		if(_size+len > _buf_len) //overload
		{
			_size = 0;//clear _buf
		}

		if(_size == 0)
		{
			size_t head_len= 0;
			pbuf = Header(stream, len, &head_len);
			
			if(pbuf != nullptr)
			{
				size_t head_start = pbuf-stream-head_len;
				pbuf = Payload(pbuf, len-head_start-head_len, &_payload_len);
				//head field, paylaod_len field.
				long nleft = len-head_start-head_len-pack_t::LenField;
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
					//copy form start of head to stream's end
					size_t hplen = head_len;
					if(nleft >= 0)
						hplen += pack_t::LenField;
					
					memcpy(_buf, pbuf-hplen, len-(pbuf-hplen-stream));
					_size = len;
					pbuf = nullptr;
				}
				//complete pack
			}
			//if did not found header then drop the datas
		}
		else
		{
			size_t head_len= 0;
			memcpy(_buf+_size, stream, len);
			_size += len;
			pbuf = Header(_buf, _size, &head_len);
			if(pbuf != nullptr)
			{
				size_t head_start = pbuf-_buf-head_len;
				pbuf = Payload(pbuf, len-head_start-head_len, &_payload_len);
				size_t plen = _payload_len + head_len + pack_t::LenField;
				if(_size < plen)
					pbuf = nullptr;
				else
					_size -= plen; 
			}
		}

		//cout<<"_paylod_len:"<<_payload_len<<",_size:"<<_size<<endl;
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

extern const char* Head0xff(const char *stream, size_t len, size_t *head_len);

NAMESP_END

#endif /*PACK_H*/
