#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <list>
#include <memory>

#ifndef MVC_OBJECT_H
#include "mvcobject.h"
#endif

using namespace std;

class Member
{
public:
	Member()
		:_id("")
		, _name("")
	{}

	Member(const string &id, const string &name)
		:_id(id)
		, _name(name)
	{}

	Member(const char *id, const char *name)
		:_id(id)
		, _name(name)
	{}

	string Name(){ return _name; }
	string Id(){ return _id; }
	
private:
	string _name;
	string _id;
};

typedef arsee::Object<Member> member_obj_t;
typedef shared_ptr<member_obj_t> member_ptr_t;
typedef list< member_ptr_t > member_list_t;

//class GetMemberData
//{
//public:
//	void Solve(const member_obj_t *obj, string &id, string &name);
//};
//
//class SetMemberData
//{
//public:
//	void Solve(member_obj_t *obj, const string &id, const string &name);
//};
//
//class GetMemberListData
//{
//public:
//	void Solve(const member_list_obj_t *obj, const string &id, member_ptr_t &mem);
//};
//
//
//class SetMemberListData
//{
//public:
//	void Solve(member_list_obj_t *obj, member_ptr_t &mem);
//};

typedef arsee::Object<member_list_t> member_list_obj_t;



#endif /*MEMBER_H*/