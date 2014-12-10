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
	
	size_t Header()override
	{
		return Head0xff(_head);
	}
};

class JUnSerializer:
	public UnSerializerAbstr<JSerializer, JUnSerializer>
{
public:
	JUnSerializer(size_t buf_len)
		:UnSerializerAbstr<JSerializer, JUnSerializer>(buf_len)
	{
	}

	int Parse(pack_t &pck, stream_t &stream)override
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
	
	const char* Header(const char* stream, size_t len, size_t *head_len)
	{
		return Head0xff(stream, len, head_len);
	}
};

typedef Pack<JSerializer, JUnSerializer> Jpack;


NAMESP_END

#endif /*JPACK_H*/
