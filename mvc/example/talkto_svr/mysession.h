//mysession.h

namespace net
{
	class Session{};
}

//#include "net/session.h"
#include "jpack.h"
#include "controls.h"
#include "globaldef.h"

using namespace arsee;

template<class Pack, class ObjCollection, class... Dispachters>
class MySession :
	public net::Session
{
	typedef Pack pack_t;
	
public:
	void InputHandle()
	{
		typename pack_t::unserializer_t unserializer_t;
		auto fuc = std::bind(unserializer_t, std::placeholders::_1);
		pack_t pck = std::move(fuc(pack_t::stream_t(buf)));

		ArgIteration<Dispachters...>::Handle(ObjCollection::Instance(), pck, _replies);
	}

	void OutputHandle()
	{
		for (auto &ip : replies)
		{
			typename pack_t::serializer_t serializer_t;
			auto fuc = std::bind(serializer_t, pck, std::placeholders::_1);
			size_t len = 0;
			typename pack_t::stream_t stream = fuc(len);
			_outbuf = stream.c_str();
			_outbuf_size = len;
		}
	}

private:
	std::vector<pack_t> _replies;
};