//dbmodel.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

#ifndef DBMODEL_H
#define DBMODEL_H

#if defined(_MSC_VER_)
#pragma once
#endif

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <tuple>

#ifndef STRINGEX_H
#include "../stringex.h"
#endif 

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef TMPS_H
#include "../tmps.h"
#endif

using namespace std;

NAMESP_BEGIN
struct Attribute{
	Attribute(bool accept_null)
	:accept_null_(accept_null)
	{
		if (accept_null_)
			isnull_ = true;
		else
			isnull_ = false;
	}

	virtual ~Attribute(){}

	virtual string str()const = 0;
	virtual void str(const char* str) = 0;
	virtual void str(const string& str) = 0;

	bool accept_null_;
	bool isnull_;
};

template<class Value>
struct AttributeG : public Attribute{
	typedef Value value_type;

	AttributeG(Value val, bool accept_null = false)
		:Attribute(accept_null)
		, value_(val)
	{
	}

	AttributeG(bool accept_null)
		:Attribute(accept_null)
	{
	}

	AttributeG<Value>& operator=(const Value& val)
	{
		this->value_ = val;
		return (*this);
	}

	operator Value(){ return value_; }

	string str()const { return t2str(value_); }
	void str(const char* str){ value_ = str2t<Value>(string(str)); }
	void str(const string& str){ value_ = str2t<Value>(str); }

	Value value_;
};

template<class Value, bool accept_null>
struct AttributeT;

template<class Value>
struct AttributeT<Value, false> : public AttributeG<Value>{
	AttributeT(Value val)
	:AttributeG<Value>(val, false)
	{}

	AttributeT()
		:AttributeG<Value>(Value(), false)
	{}

	static shared_ptr<Attribute> New(Value val)
	{
		return shared_ptr<Attribute>(new AttributeT<Value, false>(val));
	}

	AttributeT<Value, false>& operator=(const Value& val)
	{
		this->value_ = val;
		return (*this);
	}
};

template<class Value>
struct AttributeT<Value, true> : public AttributeG<Value>{
	AttributeT()
	:AttributeG<Value>(true)
	{}

	static shared_ptr<Attribute> New()
	{
		return shared_ptr<Attribute>(new AttributeT<Value, true>);
	}

	AttributeT<Value, true>& operator=(const Value& val)
	{
		this->value_ = val;
		return (*this);
	}
};


typedef AttributeT<int, false> int_attr_t;
typedef AttributeT<size_t, false> uint_attr_t;
typedef AttributeT<float, false> float_attr_t;
typedef AttributeT<double, false> double_attr_t;
typedef AttributeT<string, false> str_attr_t;

typedef AttributeT<int, true > int_null_attr_t;
typedef AttributeT<size_t, true > uint_null_attr_t;
typedef AttributeT<float, true > float_null_attr_t;
typedef AttributeT<double, true > double_null_attr_t;
typedef AttributeT<string, true > str_null_attr_t;


////////////////////////////////////////////////////////////////
//DbModel
template<class... Attributes>
class DbModel
{
public:
	typedef vector< shared_ptr<DbModel> > objlist_t;
	typedef tuple<Attributes...> attr_map_t;
	//typedef attr_map_t::iterator attr_iterator;
	
	
			
public:
	DbModel(Attributes&... attrs)
	{
		_attrs = make_tuple(attrs...);
	}

	DbModel(Attributes&&... attrs)
		:DbModel(attrs...)
	{}
	
	virtual ~DbModel()
	{
	}
	
	//virtual string get_table()const=0;	

	string get_params()const;
	string get_values()const;
	Attribute* get_attr(const string& key)const;
	void set_attr(const string& key, const Attribute* val);
	void set_attr(const string& key, const char* val){ set_attr(key, string(val)); }
	
	template<class Value>
	void set_attr(const string& key, Value val)
	{
		size_t idx=-1;
		Iteration<AttrCount, AttrAt>::Handle(key, &idx);
		if( idx > -1 )
			set_attr<idx>(val);
		else
			throw exception(string("attribute["+key+"] not found").c_str());
	}
	
	//attr_iterator& attr_begin(){ return _attrs.begin(); }
	
	
	/*auto get_attr(const string& key)const ->decltype( get_attr<0>() )
	{
		size_t idx = -1;
		Iteration<AttrCount, AttrAt>::Handle(key, &idx);
		if (idx > -1)
			return get_attr<idx>();
		else
			throw exception(string("attribute[" + key + "] not found").c_str());
	}
	*/
	template<size_t Idx, class Value>
	void set_attr(Value)
	{
		//if idx > AttrCount, what happend will be?
		auto& attr = get<Idx>(_attrs);
		attr = Value;
	}
	
	template<size_t Idx>
	auto get_attr()const ->decltype( get<Idx>(_attrs).value_ )
	{
		auto& attr = get<Idx>(_attrs);
		return attr.value_;
		
		throw exception(string("attribute[" + t2str(Idx) + "] not found").c_str());
	}
	

protected:
	enum{ AttrCount = ArgCounter<Attributes...>::value };
	
	template<int N>
	struct AttrAt
	{
		void operator()(std::string &name, size_t* idx)
		{			
			if ( ArgAt<N, Attributes...>::result::name() == name )
				idx = N;
		}
	};	
	
	attr_map_t _attrs;

};


NAMESP_END

#endif /*MODEL_H*/