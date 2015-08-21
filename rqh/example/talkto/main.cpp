#include "views.h"
#include "controls.h"
#include "logics.h"


int main()
{
	membermanage_view_t mmview;
	member_list_obj_t memlist;
	
	member_list_ctrl_t mem_ctrl(&memlist, new AddMember());

	mmview.ref().AddMember(mem_ctrl, "Lily", "Lily");

	member_view_t mview;
	
	member_login_ctrl_t login(&memlist, new MemberLogin(), new member_list_rsp_t(new MemberLoginFilter<member_view_t>()));
	login.AttachView(&mview);

	mview.ref().Login(login, "Lily", "abc");

	system("pause");
	return 0;
}