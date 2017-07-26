//class_info_base.h
//copyright	: Copyright (c) 2015 arsee.
//license	: GNU GPL v2.
//author	: arsee

#ifndef CLASS_INFO_BASE_H
#define CLASS_INFO_BASE_H

#include <map>
#include <string>
#include <memory>

#ifndef CLSS_CREATION_REACTOR_H
#include "class_creation_reactor.h"
#endif


#ifndef NAMESPDEF_H
#include "namespdef.h"
#endif

#define BUILD_ATTR(TYPE, NAME) \
        TYPE get_##NAME(){ return NAME; }\
        void set_##NAME(const TYPE& val){ /*cout<<#NAME<<"="<<val<<endl;*/NAME = val; }\
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
	public:\
	ClassInfoBase* get_class_info(){ return &class_info;}\
	private:\
	ClassInfo<NAME,class_##NAME> class_info;\
	};

NAMESP_BEGIN


struct ClassInfoBase
{
	struct AttrWrapperBase{
		virtual void* get() = 0;
		virtual void set(const char* val) = 0;
		virtual int sizeof_() = 0;
		virtual ~AttrWrapperBase(){};
		virtual std::string typeof_()=0;
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

template<class T>
struct Typeof
{
	std::string operator()()
	{
		T t;
		ClassInfoBase* ci = t.get_class_info();
		return ci->get_class_name();
	}
};

template<class T>
struct Typeof<vector<T> >
{
	std::string operator()()
	{
		return typeid(vector<T>).name(); 
	}
};

template<class T>
struct Typeof<shared_ptr<T> >
{
	std::string operator()()
	{
		return typeid(shared_ptr<T>).name(); 
	}
};

template<>
struct Typeof<int>
{
	std::string operator()()
	{
		return typeid(int).name(); 
	}
};

template<>
struct Typeof<float>
{
	std::string operator()()
	{
		return typeid(float).name(); 
	}
};

NAMESP_END/*namespace*/

#endif /*CLASS_INFO_BASE_H*/
