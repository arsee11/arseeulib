#pragma once
#ifndef LOGICS_H
#define LOGICS_H

#ifndef MVC_REQUEST_H
#include "mvcrequest.h"
#endif

#ifndef MEMBER_H
#include "member.h"
#endif

#ifndef MVC_RESPONSE_H
#include "mvcresponse.h"
#endif

class AddMember :
	public arsee::LogicT<member_list_obj_t>
{
public:
	virtual int Execute(obj_t *obj);
	int Execute(obj_t *obj, const string &id, const string& name);
};

class MemberLogin
{
public:
	int Execute(member_list_obj_t *obj, const string &id, const string& key);
};



#endif/*LOGICS_H*/