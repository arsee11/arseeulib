//file: jpack.h

#ifndef JPACK_H
#define JPACK_H

#ifndef GLOBALDEF_H
#include "globaldef.h"
#endif

#include <string>
#include <hash_map>

#include "E:\lib\jsonlib\json.h"

NAMESP_BEGIN

//Json pack defines
//{"source":"src","target":"t","action":"response", "params":[{"name":"name1","value":"value1"},{"name":"name2","value":"value2"}]}
class Jpack
{
public:
	typedef Jpack pack_t;
	typedef std::hash_map<std::string, std::string> params_pack_t;
	typedef std::string stream_t;
	
	Jpack(){}
	
	class Serializer
	{
	public:
		Jpack::stream_t operator()(const Jpack::pack_t &pck, size_t &stream_len);
	};
	
	class UnSerializer
	{
	public:
		Jpack::pack_t operator()(const Jpack::stream_t &stream);
	};
	
	typedef UnSerializer unserializer_t;
	typedef Serializer   serializer_t;
	
public:
	Jpack(std::string src, std::string trgt, std::string act, std::string param)
		:_source(src)
		,_target(trgt)
		,_action(act)
	{
		_params["msg"] = param;
	}
	
	Jpack(std::string src, std::string trgt, std::string act, params_pack_t &params)
		:_source(src)
		,_target(trgt)
		,_action(act)
		,_params(params)
	{
	}
	
	std::string Action()const{ return std::move(_action); }
	std::string Target()const{ return std::move(_target); }
	std::string Source()const{ return std::move(_source); }
	params_pack_t Params()const{return std::move(_params);}

private:
	std::string _action="";
	std::string _target="";
	std::string _source="";
	params_pack_t _params;
};



NAMESP_END

#endif /*JPACK_H*/