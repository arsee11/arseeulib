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
#endif

#ifndef DISPATCHER_H
#include "dispatcher.h"
#endif

#ifndef CHANNEL_H
#include "channel.h"
#endif

#ifndef MEMBER_H
#include "member.h"
#endif

#ifndef LOGICS_H
#include "logics.h"
#endif

using namespace arsee;

typedef RControl<member_list_obj_t, MemberLogin, RRequest> member_login_ctrl_t;
typedef member_login_ctrl_t::response_t member_login_rsp_t;
typedef Dispatcher<member_login_ctrl_t> member_login_dispth;


typedef RControl<member_list_obj_t, AddMember, RRequest> member_list_ctrl_t;
typedef member_list_ctrl_t::response_t member_list_rsp_t;
typedef Dispatcher<member_list_ctrl_t> member_list_dispth;


//typedef arsee::RControl<member_obj_t, AddMember, arsee::RRequest, udpchannel_t> member_ctrl_t;
//typedef member_list_ctrl_t::response_t member_list_rsp_t;
//typedef Dispatcher<member_ctrl_t> member_dispth;


#ifndef MSG_H
#include "msg.h"
#endif

//typedef arsee::Control<msg_obj_t, arsee::Request, arsee::Response> msg_ctrl_t;
//typedef msg_ctrl_t::request_t msg_rqt_t;
//typedef msg_ctrl_t::response_t msg_rsp_t;



#endif /*CONTROLS_H*/
