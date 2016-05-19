#include "member.h"

////GetMemberListData
//void GetMemberListData::Solve(const member_list_obj_t *obj, const string &id, member_ptr_t &mem)
//{
//	auto i = find_if(obj->ref().begin(), obj->ref().end(),
//		[&id](const member_ptr_t &mem){ return mem->ref().Id() == id; }
//	);
//
//	if (i != obj->ref().end())
//	{
//		mem = (*i);
//	}
//	else
//	{
//		mem = member_ptr_t(nullptr);
//	}
//
//}
//
//
////SetMemberListData
//void SetMemberListData::Solve(member_list_obj_t *obj, member_ptr_t &mem)
//{
//	obj->ref().push_back(mem);
//}
//
//
////GetMemberData
//void GetMemberData::Solve(const member_obj_t *obj, string &id, string &name)
//{
//	id = obj->ref()._id;
//	name = obj->ref()._name;
//}
//
//
////SetMemberData
//void SetMemberData::Solve(member_obj_t *obj, const string &id, const string &name)
//{
//	obj->ref()._id = id;
//	obj->ref()._name = id;
//}

