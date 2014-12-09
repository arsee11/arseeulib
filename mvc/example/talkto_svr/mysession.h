//mysession.h

namespace net
{
	class Session{};
}

//#include "net/session.h"
#include "jpack.h"
#include "controls.h"
#include "globaldef.h"
#include "net/session.h"

using namespace arsee;

template<class Pack, class ObjCollection, class... Dispachters>
class MySession :
	//public net::Session
	public Session
{
	typedef Pack pack_t;
	
public:
	MySession(fd_t fd, const char *ip, unsigned short port)
		:Session(fd, ip, port)
	{}
	void InputHandle(size_t len)
	{
		typename pack_t::unserializer_t unserializer_t;
		auto fuc = std::bind(unserializer_t, std::placeholders::_1);
		pack_t pck = std::move(fuc(typename pack_t::stream_t(_inbuf)));
		if(pck.Status() )
		{
			ArgIteration<Dispachters...>::Handle(ObjCollection::Instance(), pck, _replies);
		}
	}

	void OutputHandle()
	{
		for (auto &ip : _replies)
		{
			typename pack_t::serializer_t serializer_t;
			auto fuc = std::bind(serializer_t, ip, std::placeholders::_1);
			size_t len = 0;
			typename pack_t::stream_t stream = fuc(len);
			_outbuf = const_cast<char*>(stream.c_str());
			_outbuf_size = len;
		}
	}

private:
	std::vector<pack_t> _replies;
};
