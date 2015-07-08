#pragma

#ifndef VIEWS_H
#define VIEWS_H

#include <string>

using namespace std;

#ifndef MVCVIEW_ABSTR_H
#include "mvcview_abstr.h"
#endif

#ifndef VIEW_H
#include "mvcview.h"
#endif

#include <iostream>

/////////////////////////////////////////////////////////////////////
//MsgView
class MsgView :
	public arsee::IView
{
public:
	void Output() override;
	void Input(const string &msg);
};

typedef arsee::View<MsgView> msgview_t;


/////////////////////////////////////////////////////////////////////
//MemberManageView
class MemberView :
	public arsee::IView
{
public:
	void Output() override;
	bool Receive(const string &id, const string &newname);
	bool Receive(const string &notify);

	template<class CONTROL>
	void Login(const CONTROL &ctrl, const string &id, const string &key)
	{
		if (ctrl.Execute(id, key) == 0)
			cout << "Logined!" << endl;
		else
			cout << "Login failed!" << endl;
	}

	bool SendMsg(const string &to, const string &msg);
	bool SendMsg(const string &msg);

	string notify_msg;
};

typedef arsee::View<MemberView> member_view_t;


/////////////////////////////////////////////////////////////////////
//MemberManageView
class MemberManageView :
	public arsee::IView
{
public:
	void Output() override;
	bool Receive(const string &id, const string &newname){}

	template<class CONTROL>
	bool AddMember(CONTROL &ctrl, const string &name, const string &id)
	{
		ctrl.Execute(name, id);
		return true;
	}

	bool RemoveMember(const string &id);
	bool EditMember(const string &id, const string &newname);
	
};

typedef arsee::View<MemberManageView> membermanage_view_t;

#endif /*VIEWS_H*/