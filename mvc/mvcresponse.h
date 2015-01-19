//file: mvcresponse_abstr.h

#ifndef MVCRESPONSE_H
#define MVCRESPONSE_H


#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN

template<class View, class Pack>
class Response
{
protected:
	typedef Response<View, Pack> base_t;

public:
	typedef View view_t;
	typedef Pack pack_t;

public:
	static Response* CreateRomote(const std::string &name, view_t &view);
	static Response* CreateLocal(const std::string &name, view_t *view);
	
	static Response* CreateRomote(std::string &&name, view_t &view)
	{
		return CreateRomote(name, view);
	}
	
	static Response* CreateLocal(std::string &&name, view_t *view)
	{
		return CreateLocal(name, view);
	}
	
	std::string name(){ return _name; }
	
	virtual ~Response(){}
	
	Response(const std::string &name)
		:_name(name)
	{
	}
	
	Response(std::string &&name)
		:Response(name)
	{}
	
protected:
	std::string _name;
	size_t _rqt_id=-1; //request id
};

NAMESP_END;
#endif /*MVCRESPONSE_H*/
