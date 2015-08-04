//responseto.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee


#ifndef LRESPONSE_H
#define LRESPONSE_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef UTILITY_H
#include "utility.h"
#endif

#ifndef INVOKER_H
#include "invoker.h"
#endif

NAMESP_BEGIN

////////////////////////////////////////////////
//Local Response abstract 
//for client use
template<class Pack>
class LResponse
{
public:
	std::string name(){ return _name; }

	virtual ~LResponse(){}

	virtual void Update(const Pack& pck) = 0;

	//Pase the msg buf to Pack
	static bool Parse(const char* buf, size_t len, typename Pack::pack_list_t& pcks)
	{
		typename Pack::pack_ptr_t pck = typename Pack::pack_ptr_t( new Pack );
		typename Pack::unserial_t us(1024);
		if (us(*pck, buf, len) > 0)
		{
			if (pck->status())
				pcks.push_back(pck);
		}

		pck = typename Pack::pack_ptr_t(new Pack);
		while (us(*pck, nullptr, 0) > 0)
		{
			if (pck->status())
				pcks.push_back(pck);

			pck = typename Pack::pack_ptr_t(new Pack);
		}
		
		//these for test
		//pck = Pack("tester", "msgview", "response");
		//pck.param("msg", "hello, it just a test!");
		//pck.param("from", "007");
		return pcks.size() > 0;
			
	}
	
protected:
	LResponse(const std::string &name)
		:_name(name)
	{
	}

	LResponse(std::string &&name)
		:LResponse(name)
	{}

protected:
	std::string _name;
	size_t _rqt_id = -1; //request id
};


////////////////////////////////////////////////
///Response Template
///@View thie view attached to. Derived from Invoker.
///@Pack the message Pack.
template<class View, class Pack>
class LResponseTmp
	:public LResponse<Pack>
{
public:	
	static inline LResponse* Create(View* view)
	{
		return new LResponseTmp<View, Pack>(view);
	}

	void Update(const Pack& pck) override
	{
		if(_view != nullptr)
		{
			if (_view->name() == pck.target())
			{
				for (auto &i : pck.params())
				{
					Invoker<View::PC>::Invoke(i, _view);
				}
			}
				
		}
	}
	
protected:
	View *_view=nullptr;

private:
	LResponseTmp(View* view)
		:LResponse<Pack>(view->name())
		, _view(view)
	{
	}
};



/////////////////////////////////////////////////////////
//Factory 
class LResponseFactory
{
public:
	template<class View, class Pack>
	static inline LResponse<Pack>* Create(View* view)
	{
		return LResponseTmp<View, Pack>::Create(view);
	}

};

NAMESP_END

#endif/*LRESPONSE_H*/
