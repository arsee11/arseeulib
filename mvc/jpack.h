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
class JUnSerializer;

class JSerializer:
	public SerializerAbstr<JSerializer, JUnSerializer>
{
private:
	stream_t Resolve(const pack_t &pck)override
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
		std::string str = wr.write(root);
		return std::move(str);		
	}
	
	size_t Header()override
	{
		_head = new char[sizeof(long)];
		memset(_head, 0xff, sizeof(long));
		return sizeof(long);
	}
};

class JUnSerializer:
	public UnSerializerAbstr<JSerializer, JUnSerializer>
{
public:
	int Parse(pack_t &pck, const char *stream)override
	{
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
			return 1;
		}
		
		return 0;
	}
	
	const char* Header(const char* stream, size_t len, size_t *head_len)
	{
		long head = 0;
		memset(&head, 0xff, sizeof(long);
		*head_len = sizeof(long);
		for(size_t i=0; i<len-sizeof(long)-1; ++i)
		{
			long tmp = *(long*)(stream+i);
			if(tmp == head)
				return tmp+4;
		}

		return nullptr;
	}
};

typedef Pack<JSerializer, JUnserializer> Jpack;


NAMESP_BEGIN

#endif /*JPACK_H*/
