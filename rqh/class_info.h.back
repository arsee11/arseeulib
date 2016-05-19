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

#ifndef CLSS_CREATION_REACTOR_H
#include "class_creation_reactor.h"
#endif

#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

#define BUILD_ATTR(TYPE, NAME) \
        TYPE get_##NAME(){ return NAME; }\
        void set_##NAME(const TYPE& val){ NAME = val; }\
        TYPE NAME

#define REGISTER_ATTR(CLASS, TYPE, NAME) \
        class_info.add_attr<TYPE, &CLASS::get_##NAME, &CLASS::set_##NAME>(#NAME, this)
		
#define DECLARE_CLASS_NAME(NAME) \
	extern char const class_##NAME[]

#define DEF_CLASS_NAME(NAME) \
	char const class_##NAME[]=#NAME 

#define DEF_CLASS_INHERIT_BEGIN(NAME, BASE) \
	extern char const class_##NAME[]; \
	struct NAME : public BASE \
	{

#define DEF_CLASS_BEGIN(NAME) \
	extern char const class_##NAME[]; \
	struct NAME \
	{

#define DEF_CLASS_END(NAME) \
	ClassInfo<NAME,class_##NAME> class_info;\
	};

NAMESP_BEGIN

struct ClassInfoBase
{
	struct AttrWrapperBase{
		virtual std::shared_ptr<char> get() = 0;
		virtual void set(const char* val) = 0;
		virtual int sizeof_() = 0;
		virtual ~AttrWrapperBase(){};
	};

	typedef std::shared_ptr<AttrWrapperBase> attr_wrapper_ptr_t;
	typedef std::map<std::string, attr_wrapper_ptr_t> attr_map_t;
	typedef attr_map_t::iterator attr_iterator;
	attr_map_t attrs;

	virtual const char* get_class_name()=0;
	
	static void* get_object(const char* class_name)
	{	
		return ClassFactory::instance().Create(class_name);
	}

	virtual ~ClassInfoBase()
	{
		
	}
};

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
			this->attr_name = attr_name;
			obj = o;
		}

		void geti(value_type& val){ val= (obj->*Getter)(); }
		void seti(const value_type& val){
			return (obj->*Setter)(val); }

		std::shared_ptr<char> get()override
		{
			char *buf = new char[sizeof_()];
			memset(buf, 0, sizeof_());
			const value_type& val = (obj->*Getter)();
			memcpy(buf, &val, sizeof_()); 
			return std::shared_ptr<char>(buf);
		}
		void set(const char* val)override{ return seti(*(value_type*)val); }
		int sizeof_() override { return sizeof(value_type); }
		Class *obj = nullptr;
		std::string attr_name;
	};

	template<std::string(Class::*Getter)(), void (Class::*Setter)(const std::string&)>
	struct AttrWrapper<string, Getter, Setter> :public AttrWrapperBase{
		typedef std::string value_type;

		AttrWrapper(Class* o, const std::string& attr_name)
		{
			this->attr_name = attr_name;
			obj = o;
		}

		void geti(value_type& val){ val = (obj->*Getter)(); }
		void seti(const value_type& val){
			return (obj->*Setter)(val);
		}

		std::shared_ptr<char> get()override
		{
			const value_type& val = (obj->*Getter)();
			char *buf = new char[val.size()];
			memset(buf, 0, val.size());
			memcpy(buf, val.c_str(), val.size());
			return std::shared_ptr<char>(buf);
		}
		void set(const char* val)override{ return seti(val); }
		int sizeof_() override { return 0; }
		Class *obj = nullptr;
		std::string attr_name;
	};

	template<class Type, Type(Class::*Getter)(), void(Class::*Setter)(const Type&)>
	void add_attr(const char *attr_name, Class* obj)
	{
		attrs[attr_name] = ClassInfoBase::attr_wrapper_ptr_t(
			new AttrWrapper<Type, Getter, Setter>(obj, attr_name));
	}

	operator int(){ return 0; }
	ClassInfo<Class, class_name> operator=(int rhs){ return *this; }
	const char* get_class_name(){ return class_name; }
	
	const static Register<Class, class_name> r;

};


template<class Class, const char* class_name>
const Register<Class, class_name> ClassInfo<Class, class_name>::r;

NAMESP_END/*namespace*/

#endif /*CLASS_INFO_H*/
