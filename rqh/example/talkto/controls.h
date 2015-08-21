#pragma once

#ifndef CONTROLS_H
#define CONTROLS_H

#ifndef MVC_RESPONSE_H
#include "mvcresponse.h"
#endif

#ifndef MVC_REQUEST_H
#include "mvcrequest.h"
#endif

#ifndef MVC_CONTROL_H
#include "mvccontrol.h"
#endif;


#ifndef MEMBER_H
#include "member.h"
#endif

#ifndef LOGICS_H
#include "logics.h"
#endif

typedef arsee::Control<member_list_obj_t, MemberLogin, arsee::Request> member_login_ctrl_t;
typedef member_login_ctrl_t::response_t member_login_rsp_t;

template<class VIEW>
class MemberLoginFilter :
	public member_login_rsp_t::FilterT<VIEW>
{
	virtual bool Transform(view_t * view, obj_t *obj) override
	{
		return view->Receive("Some body login. Welcome!");
	}
};


typedef arsee::Control<member_list_obj_t, AddMember, arsee::Request> member_list_ctrl_t;
typedef member_list_ctrl_t::response_t member_list_rsp_t;


typedef arsee::Control<member_obj_t, AddMember, arsee::Request> member_ctrl_t;
typedef member_list_ctrl_t::response_t member_list_rsp_t;

template<class VIEW>
class MemberFilter :
	public member_list_rsp_t::FilterT<VIEW>
{
	virtual bool Transform(view_t * view, obj_t *obj) override
	{
		//view->Receive(obj->ref().Id(), obj->ref().Name());
	}
};

#ifndef MSG_H
#include "msg.h"
#endif

//typedef arsee::Control<msg_obj_t, arsee::Request, arsee::Response> msg_ctrl_t;
//typedef msg_ctrl_t::request_t msg_rqt_t;
//typedef msg_ctrl_t::response_t msg_rsp_t;



#endif /*CONTROLS_H*/