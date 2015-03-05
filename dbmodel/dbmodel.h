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

#ifndef STRINGEX_H
#include "../stringex.h"
#endif 

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

using namespace std;

NAMESP_BEGIN

class DbModel
{
	struct Attribute{
		Attribute(bool accept_null)
			:accept_null_(accept_null)
		{
			if( accept_null_)
				isnull_ = true;
			else
				isnull_ = false;
		}

		virtual ~Attribute(){}

		virtual string str()const =0;
		virtual void str(const char* str)=0;
		virtual void str(const string& str)=0;

		bool accept_null_;
		bool isnull_;
	};
	
	template<class Value>
	struct AttributeG : public Attribute{
		typedef Value value_type;

		AttributeG(Value val, bool accept_null=false)
			:Attribute(accept_null)
			,value_(val)
		{
		}
		
		AttributeG(bool accept_null)
			:Attribute(accept_null)
		{
		}
		
		~AttributeG()
		{
			cout << "~AttributeG()" << endl;
		}

		AttributeG<Value>& operator=(const Value& val)
		{
			this->value_ = val;
			return (*this);
		}
		
		operator Value(){ return value_; }
		
		string str()const { return t2str( value_ ); }
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

		~AttributeT()
		{
			cout << "~AttributeT<Value, false>()" << endl;
		}

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

		~AttributeT()
		{
			cout << "~AttributeT<Value, true>()" << endl;
		}

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
	
	
public:
	typedef vector< shared_ptr<DbModel> > objlist_t;
	typedef map<string, shared_ptr<Attribute> > attr_map_t;
	typedef attr_map_t::iterator attr_iterator;
	
	typedef AttributeT<int   , false> int_attr_t;
	typedef AttributeT<size_t, false> uint_attr_t;
	typedef AttributeT<float , false> float_attr_t;
	typedef AttributeT<double, false> double_attr_t;
	typedef AttributeT<string, false> str_attr_t;
	
	typedef AttributeT<int	, true > int_null_attr_t;
	typedef AttributeT<size_t, true > uint_null_attr_t;
	typedef AttributeT<float	, true > float_null_attr_t;
	typedef AttributeT<double, true > double_null_attr_t;
	typedef AttributeT<string, true > str_null_attr_t;
			
public:
	virtual ~DbModel()
	{
	}
	
	virtual string get_table()const=0;	

	string get_params()const;
	string get_values()const;
	Attribute* get_attr(const string& key)const;
	void set_attr(const string& key, const Attribute* val);
	void set_attr(const string& key, const string& val);
	void set_attr(const string& key, const char* val){ set_attr(key, string(val)); }
	
	template<class Value>
	void set_attr(const string& key, Value val)
	{
		attr_map_t::iterator i = _attributes.find(key);
		if(i != _attributes.end() )
		{
			if (i->second->accept_null_)
			{
				((AttributeT<Value, true>&)(*(i->second))) = val;
				//static_cast<AttributeT<Value, true>*>(i->second.get())->value_ = val;
				i->second->isnull_ = false;
			}
			else
				( (AttributeT<Value, false>&)(*(i->second)) ) = val;

			return;
		}

		throw exception(string("attribute["+key+"] not found").c_str());
	}
	
	attr_iterator& attr_begin(){ return _attributes.begin(); }
	
	template<class T>
	T get_attr(const string& key)const
	{
		attr_map_t::const_iterator i = _attributes.find(key);
		if (i != _attributes.end())
		{
			if (i->second->accept_null_)
			{
				AttributeT<T, true>* val = static_cast<AttributeT<T, true>*>(i->second.get());
				return *val;
			}
				
			AttributeT<T, false>* val = static_cast<AttributeT<T, false>*>(i->second.get());
			return *val;
			//return *( static_cast<AttributeT<T, false>*>(i->second.get()) );
		}

		throw exception(string("attribute[" + key + "] not found").c_str());
	}


protected:
	attr_map_t _attributes;

};


typedef DbModel::int_attr_t		int_attr_t;
typedef DbModel::uint_attr_t	uint_attr_t;
typedef DbModel::float_attr_t	float_attr_t;
typedef DbModel::double_attr_t	double_attr_t;
typedef DbModel::str_attr_t		str_attr_t;

typedef DbModel::int_null_attr_t	int_null_attr_t;
typedef DbModel::uint_null_attr_t	uint_null_attr_t;
typedef DbModel::float_null_attr_t	float_null_attr_t;
typedef DbModel::double_null_attr_t	double_null_attr_t;
typedef DbModel::str_null_attr_t	str_null_attr_t;

NAMESP_END

#endif /*MODEL_H*/