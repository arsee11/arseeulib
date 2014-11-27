//file: jpack.cpp

#include "jpack.h"
#include <utility>
#include <json/json.h>

NAMESP_BEGIN


Jpack::pack_t Jpack::UnSerializer::operator()(const std::string &stream)
{
	pack_t pck;
	Json::Reader rd;
	Json::Value root;
	
	if( rd.parse( stream, root, false) )
	{
		pck._source = root["source"].asString();
		pck._action = root["action"].asString();
		pck._target = root["target"].asString();
		Json::Value params = root["params"];
		for(int i=0; i<params.size(); i++)
		{
			Json::Value param = params[i];
			pck._params[ param["name"].asString() ] = param["value"].asString();
		}
	}
	
	return std::move<pack_t&>(pck);
}

Jpack::stream_t Jpack::Serializer::operator()(const Jpack::pack_t &pck, size_t &stream_len)
{
	Json::FastWriter wr;
	Json::Value root;
	root["source"] = pck._source;
	root["target"] = pck._target;
	root["action"] = pck._action;
	Json::Value param;
	Json::Value params;
	for(auto &i : pck._params)
	{
		param["name"]  = i.first;
		param["value"] = i.second;
		params.append(param);
	}
			
	root["params"] = params;
	Jpack::stream_t stream = wr.write(root);
	stream_len = stream.size();
	return std::move( stream );
}

NAMESP_END
