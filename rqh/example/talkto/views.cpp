#include "views.h"

#ifndef CONTROLS_H
#include "controls.h"
#endif

#include "logics.h"


/////////////////////////////////////////////////////////////////////
//MemberView
bool MemberView::SendMsg(const string &to, const string &msg)
{
	return true;
}

bool MemberView::SendMsg(const string &msg)
{
	return true;
}
void MemberView::Output()
{
	cout << notify_msg << endl;
}

bool MemberView::Receive(const string &id, const string &newname)
{
	return true;
}

bool MemberView::Receive(const string &notify)
{
	notify_msg = notify;
	return true;
}

/////////////////////////////////////////////////////////////////////
//MemberManageView
void MemberManageView::Output()
{

}

//template<class CONTROL>
//bool MemberManageView::AddMember(CONTROL &ctrl, const string &name, const string &id)
//{
//	member_ptr_t mem = member_ptr_t(new member_obj_t("Lily", "Lily"));;
//	ctrl.Execute(mem);
//	return true;
//
//}

bool MemberManageView::RemoveMember(const string &id)
{
	return false;

}

bool MemberManageView::EditMember(const string &id, const string &newname)
{
	return false;
}