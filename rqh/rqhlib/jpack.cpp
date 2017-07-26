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

NAMESP_BEGIN

const char* Head0xff(BufferWrapper& buf, size_t *head_len)
{
	int head = 0;
	memset(&head, 0xff, 4);
	*head_len = 4;
	if( len > 4 )
	{
		for(size_t i=0; i<=len-4; ++i)
		{
			int tmp = *(int*)(_buf+i);
			if(tmp == head)
				return _buf+i+4;
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
	Json::FastWriter wr;
	Json::Value root;
	if( !pck.source().empty() )
		root["source"		] = pck.source();
		
	if( !pck.source().empty() )
		root["target"		] = pck.target();
	
	root["action"		] = pck.action();
	root["type"  		] = pck.type();
	if( !pck.get_continue().empty() )
		root["continue"		] = pck.get_continue();
		
	root["paramType"	] = pck.param_type();
	root["paramEncoding"] = pck.param_encoding();
	Json::Value param;
	Json::Value params;
	int k = 0;
	for (auto &i : pck.params())
	{
		Json::Value param_item;
		for (auto &j : i)
		{
			Json::Value param;
			param["name"] = j.first;
			param["value"] = j.second;
			param_item.append(param);
		}
		params["param" + t2str(k)] = param_item;
		++k;
	}

	root["params"] = params;
	return std::move(wr.write(root));
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
			pck.param_encoding	( root["paramEncoding"].asString()	);
			
			Json::Value& params = root["params"];
			for(int i=0; i<params.size(); i++)
			{
				Json::Value& param_item = params["param"+t2str(i)];
				pack_t::param_item_t ppitem;
				for(int j=0; j< param_item.size(); j++)
				{
					Json::Value& param = param_item[j];				
					ppitem[param["name"].asString()] = param["value"].asString();
				}
				pck.append_param(ppitem);
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

const char* JUnSerializer::Header(BufferWrapper& buf, size_t *head_len)
{
	return Head0xff(buf, head_len);
}

NAMESP_END

