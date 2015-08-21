#pragma once
#ifndef LOGICS_H
#define LOGICS_H

#ifndef MVC_REQUEST_H
#include "mvcrequest.h"
#endif

#ifndef MEMBER_H
#include "member.h"
#endif


#ifndef PARAMS_DISPATCH_H
#include "params_dispatch.h"
#endif

class AddMember :
	public arsee::LogicT<member_list_obj_t>
	, public arsee::BaseLogicTmpl<2, AddMember, string, string>
{
public:
	virtual int Execute(obj_t *obj);
	int Execute(obj_t *obj, const string &id, const string& name);

	const static string name;
};

class MemberLogin:
	public arsee::BaseLogicTmpl<2, MemberLogin, string, string>
{
public:
	
	int Execute(member_list_obj_t *obj, const string &id, const string& key);

	const static string name;
};



#endif/*LOGICS_H*/