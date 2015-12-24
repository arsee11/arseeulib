//file: jpack.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify	
//data	: 2014-12-10
//log	: modify 
//****************************

#include <utility>
#include "json/json.h"

#ifndef JPACK_H
#include "../jpack.h"
#endif

#ifndef STRINGEX_H
#include "../../stringex.h"
#endif

#ifndef CLASS_SERAILIZE_PATTERN_H
#include "../../class_serialize_pattern.h"
#endif

NAMESP_BEGIN

const char* Head0xff(const char *stream, size_t len, size_t *head_len)
{
	int head = 0;
	memset(&head, 0xff, 4);
	*head_len = 4;
	if( len > 4 )
	{
		for(size_t i=0; i<=len-4; ++i)
		{
			int tmp = *(int*)(stream+i);
			if(tmp == head)
				return stream+i+4;
		}
	}

	return nullptr;
}

JSerializer::stream_t JSerializer::Resolve(const pack_ptr_t &pck)
{
	return Resolve(*(pck.get()));
}

JSerializer::stream_t JSerializer::Resolve(const pack_t &pck)
{
	string str = "{";
	if( !pck.source().empty() )
		str+="\"source\":\"" + pck.source()+"\",";
		
	if( !pck.target().empty() )
		str+="\"target\":\"" +pck.target()+"\",";
	
	str+="\"action\":\"" + pck.action()+"\",";
	str+="\"type\":\"" +pck.type()+"\",";
	if( !pck.get_continue().empty() )
		str+="\"continue\":\"," +pck.get_continue();
		
	str+="\"paramType\":\"" + pck.param_type()+"\",";
	str+="\"paramSchema\":\"" + pck.param_schema()+"\",";
	str+="\"paramEncoding\":\"" + pck.param_encoding()+"\",";
	str += "\"params\":{";
	const pack_t::object_list_t& objs = pck.object_list();
	for (size_t i=0; i<objs.size()-1; ++i)
	{
		string objstr = class_serialize(*(objs[i].get()));
		str+="\"param" + t2str(i)+"\":" +objstr+",";
	}
	if(objs.size() > 0 )
	{
		string objstr = class_serialize(*(objs[objs.size()-1].get()));
		str += "\"param" + t2str(objs.size() - 1) + "\":" + objstr;
	}
	str+="}}";
	return str;
}

size_t JSerializer::Header()
{
	return Head0xff(_head);
}

int JUnSerializer::Parse(pack_t &pck, stream_t &stream)
{
	Json::Reader rd;
	Json::Value root;
	
	if( rd.parse( stream, root, false) )
	{
		try{
			pck.source			( root["source"		].asString()	);
			pck.action			( root["action"		].asString()	);
			pck.target			( root["target"		].asString()	);
			pck.type			( root["type"		].asString()	);
			pck.set_continue	( root["continue"	].asString()	);
			pck.param_type		( root["paramType"	].asString()	);
			pck.param_schema	( root["paramSchema"].asString()	);
			pck.param_encoding	( root["paramEncoding"].asString()	);
			
			Json::Value& params = root["params"];
			for(int i=0; i<params.size(); i++)
			{
				Json::Value& param_item = params["param"+t2str(i)];
				string name = param_item["className"].asString();
				IObject* obj = class_unserialize<IObject>(name, param_item);
				if(obj != nullptr)
					pck.add_object( pack_t::object_ptr_t(obj));
			}

			return 1;
		}
		catch(exception& e){
			//pck.error(e.what());
			return 0;
		}
	}
	
	return 0;
}

const char* JUnSerializer::Header(const char* stream, size_t len, size_t *head_len)
{
	return Head0xff(stream, len, head_len);
}

NAMESP_END

