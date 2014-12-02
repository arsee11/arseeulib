//file: jpack.h

#ifndef JPACK_H
#define JPACK_H

#include <utility>
#include <json/json.h>

#ifndef GLOBALDEF_H
#include "globaldef.h"
#endif

#ifndef PACK_H
#include "pack.h"
#endif


NAMESP_BEGIN

//Json pack defines
//{"source":"src","target":"t","action":"response", "params":[{"name":"name1","value":"value1"},{"name":"name2","value":"value2"}]}
class JSerializer
{
public:
	template<class Pack>
	typename Pack::stream_t operator()(const Pack &pck, size_t &stream_len)
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

			pck.Status(true);
		}
	}
};

class JUnSerializer
{
public:
	template<class Pack>
	Pack operator()(consti typename Pack::stream_t &stream)
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
};

typedef Pack<JSerializer, JUnserializer> Jpack;


NAMESP_BEGIN

#endif /*JPACK_H*/
