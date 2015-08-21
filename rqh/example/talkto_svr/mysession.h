//mysession.h

namespace net
{
	class Session{};
}

//#include "net/session.h"
#include "controls.h"
#include "globaldef.h"
#include "net/session.h"

using namespace arsee;

template<class Pack, class ObjCollection, class... Dispachters>
class MySession :
	//public net::Session
	public Session<1024>
{
	typedef Pack pack_t;
	
public:
	MySession(fd_t fd, const char *ip, unsigned short port)
		:Session(fd, ip, port)
	{}
	void InputHandle(size_t len)
	{
#ifdef DEBUG
		cout<<"recv("<<len<<"):"<<_inbuf<<endl;
#endif
		typename pack_t::unserial_t userial(1024);
		pack_t pck;
		userial(pck, _inbuf, len);
		if(pck.Status() )
		{
			ArgIteration<Dispachters...>::Handle(ObjCollection::Instance(), pck, _replies);
		}
	}

	void OutputHandle()
	{
		for (auto &ip : _replies)
		{
			typename pack_t::serial_t serial;
			_outbuf = serial(ip, &_outbuf_size);
		}
	}

private:
	std::vector<pack_t> _replies;
};
