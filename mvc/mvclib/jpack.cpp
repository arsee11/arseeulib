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
#include <json/json.h>

#ifndef JPACK_H
#include "../jpack.h"
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
	Json::FastWriter wr;
	Json::Value root;
	root["source"] = pck->Source();
	root["target"] = pck->Target();
	root["action"] = pck->Action();
	Json::Value param;
	Json::Value params;
	for(auto &i : pck->Params())
	{
		param["name"]  = i.first;
		param["value"] = i.second;
		params.append(param);
	}
			
	root["params"] = params;
	std::string str = wr.write(root);
	return std::move(str);		
}

JSerializer::stream_t JSerializer::Resolve(const pack_t &pck)
{
	Json::FastWriter wr;
	Json::Value root;
	root["source"] = pck.Source();
	root["target"] = pck.Target();
	root["action"] = pck.Action();
	Json::Value param;
	Json::Value params;
	for(auto &i : pck.Params())
	{
		param["name"]  = i.first;
		param["value"] = i.second;
		params.append(param);
	}
			
	root["params"] = params;
	std::string str = wr.write(root);
	return std::move(str);		
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
		pck.Source( root["source"].asString()	);
		pck.Action( root["action"].asString()	);
		pck.Target(  root["target"].asString()	);
		Json::Value params = root["params"];
		for(int i=0; i<params.size(); i++)
		{
			Json::Value param = params[i];
			pck.Param( param["name"].asString(), param["value"].asString());
		}

		pck.Status(true);
		return 1;
	}
	
	return 0;
}

const char* JUnSerializer::Header(const char* stream, size_t len, size_t *head_len)
{
	return Head0xff(stream, len, head_len);
}

NAMESP_END

