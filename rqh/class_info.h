//class_info.h
//copyright	: Copyright (c) 2015 arsee.
//license	: GNU GPL v2.
//author	: arsee

#ifndef CLASS_INFO_H
#define CLASS_INFO_H

#include <map>
#include <string>
#include <tuple>
#include <memory>

#ifndef CLASS_REGISTER_H
#include "class_register.h"
#endif

#ifndef CLASS_INFO_BASE_H
#include "class_info_base.h"
#endif

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

#ifndef UTILITYS_H
#include "../utilitys.h"
#endif


NAMESP_BEGIN


template<class Class, const char* class_name> 
struct ClassInfo :public ClassInfoBase
{
	typedef Register<Class, class_name> register_t;

	ClassInfo()
	{
		const register_t &r = ClassInfo<Class, class_name>::r;
	}

	ClassInfo(const int& other):ClassInfo(){}

	template<class Type, Type (Class::*Getter)(), void (Class::*Setter)(const Type&)>
	struct AttrWrapper :public AttrWrapperBase{
		typedef Type value_type;

		AttrWrapper(Class* o, const std::string& attr_name)
		{
//			cout<<"AttrWrapper"<<endl;
			this->attr_name = attr_name;
			obj = o;
		}
		~AttrWrapper()
		{ 
//			cout<<"~AttrWrapper"<<endl;
		}
		void geti(value_type& val){ val= (obj->*Getter)(); }
		void seti(const value_type& val){
			return (obj->*Setter)(val); }

		void* get()override
		{
			const value_type& val = (obj->*Getter)();
			value_type* pval = new value_type;
			*pval = val;
			return pval; 
		}
		void set(const char* val)override{ return seti(*(value_type*)val); }
		int sizeof_() override { return sizeof(value_type); }
		std::string typeof_()override {return Typeof<Type>()();}
		Class *obj = nullptr;
		std::string attr_name;
	};

	template<std::string(Class::*Getter)(), void (Class::*Setter)(const std::string&)>
	struct AttrWrapper<string, Getter, Setter> :public AttrWrapperBase{
		typedef std::string value_type;

		AttrWrapper(Class* o, const std::string& attr_name)
		{
//			cout<<"AttrWrapper"<<endl;
			this->attr_name = attr_name;
			obj = o;
		}

		~AttrWrapper()
		{ 
//			cout<<"~AttrWrapper"<<endl;
		}
		void geti(value_type& val){ val = (obj->*Getter)(); }
		void seti(const value_type& val){
			return (obj->*Setter)(val);
		}

		void* get()override
		{
			const value_type& val = (obj->*Getter)();
			char *buf = new char[val.size()];
			memset(buf, 0, val.size());
			memcpy(buf, val.c_str(), val.size());
			return buf;
		}
		void set(const char* val)override{ return seti(val); }
		int sizeof_() override { return 0; }
		std::string typeof_()override {return typeid(std::string).name();}

		Class *obj = nullptr;
		std::string attr_name;
	};

	template<class Type, Type(Class::*Getter)(), void(Class::*Setter)(const Type&)>
	void add_attr(const char *attr_name, Class* obj)
	{
		attrs[attr_name] = ClassInfoBase::attr_wrapper_ptr_t(
			new AttrWrapper<Type, Getter, Setter>(obj, attr_name));

		typedef typename IfElseThen<IsVector<Type, vector>::isVector == 1, 
				RegisterVector<Type>, 
				typename IfElseThen<IsSharedPtr<Type>::isSharedPtr==1, 
				RegisterSharedPtr<Type>,int>::value
				>::value register_t;

		register_t r;

	}

	operator int(){ return 0; }
	ClassInfo<Class, class_name> operator=(int rhs){ return *this; }
	const char* get_class_name()override{ return class_name; }
	
	const static Register<Class, class_name> r;

};


template<class Class, const char* class_name>
const Register<Class, class_name> ClassInfo<Class, class_name>::r;

NAMESP_END/*namespace*/

#endif /*CLASS_INFO_H*/
