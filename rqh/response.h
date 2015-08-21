//file: response_abstr.h

#ifndef RESPONSE_H
#define RESPONSE_H


#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN

///////////////////////////////////////////////////////////
//LResponsePool 
//for client use
class LResponsePool
{
public:
	static LResponsePool& instance()
	{
		static LResponsePool _myself;
		return _myself;
	}
	
	typedef std::shared_ptr<Response > rsp_ptr_t;
	
	//@rsp must be created from heap. 
	void add(const std::string& request_id, Response>* rsp)
	{
		_rsps[request_id] = rsp_ptr_t(rsp);
	}
	
	rsp_ptr_t find(const std::string& request_id)
	{
		return (*this)[request_id];
	}
	
	rsp_ptr_t find(std::string&& request_id)
	{
		return find(request_id);
	}
		
private:
	LResponsePool(){};
	~LResponsePool(){};
	LResponsePool(const LResponsePool&);
	LResponsePool& operator=(const LResponsePool&);
	
	rsp_ptr_t& operator[](const string& request_id){ return _rsps[request_id];}
	rsp_ptr_t& operator[](const string& request_id)const { return _rsps[request_id];}
	
	typedef std::map<std::string, rsp_ptr_t> rsp_pool_t;
	rsp_pool_t _rsps;
};

NAMESP_END;
#endif /*RESPONSE_H*/
