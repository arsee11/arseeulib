#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <list>
#include <memory>
#include <utility>

#ifndef MVC_OBJECT_H
#include "mvcobject.h"
#endif

using namespace std;

class Member
{
public:
	Member()
		:_id("")
		, _name("member")
	{}

	Member(Member &&other)
		:_name(std::move<string&>(other._name))
		, _id(std::move<string&>(other._id))
	{
	}

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
typedef struct member_list :public list< member_ptr_t >
{
	string Name(){ return "memberlist"; }

} member_list_t;

typedef arsee::Object<member_list_t> member_list_obj_t;
typedef arsee::ObjectsCollection<member_list_obj_t> objects_t;


#endif /*MEMBER_H*/
