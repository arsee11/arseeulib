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
	std::string name(){ return _name; }
	
	virtual ~Response(){}
	
	Response(const std::string &name)
		:_name(name)
	{
	}
	
	Response(std::string &&name)
		:Response(name)
	{}
	
	virtual void Update()=0;
	
protected:
	std::string _name;
	size_t _rqt_id=-1; //request id
};

template<class View, class Pack>
class ResponsePool
{
public:
	static ResponsePool<View, Pack>& instance()
	{
		static ResponsePool<View, Pack> _myself;
		return _myself;
	}
	
	typedef std::shared_ptr<Response<View, Pack> > rsp_ptr_t;
	
	//@rsp must be created from heap. 
	void add(const std::string& request_id, Response<View, Pack>* rsp)
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
	ResponsePool(){};
	~ResponsePool(){};
	ResponsePool(const ResponsePool<View, Pack>&);
	ResponsePool<View, Pack>& operator=(const ResponsePool<View, Pack>&);
	
	rsp_ptr_t& operator[](const string& request_id){ return _rsps[request_id];}
	rsp_ptr_t& operator[](const string& request_id)const { return _rsps[request_id];}
	
	typedef std::map<std::string, rsp_ptr_t> rsp_pool_t;
	rsp_pool_t _rsps;
};

NAMESP_END;
#endif /*MVCRESPONSE_H*/
