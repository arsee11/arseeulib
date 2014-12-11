//file: channel.h

#ifndef CHANNEL_H
#define CHANNEL_H

#ifndef GLOBALDEF_H
#include "globaldef.h"
#endif

#ifndef PACK_H
#include "pack.h"
#endif

#ifndef UDPSOCK_H
#include "../net/udpsock.h"
#endif

#include <functional>
#include <queue>

NAMESP_BEGIN


//@SOCK a socket type.
//class UdpConnection
//{
//	typedef UdpPeer::byte_t byte_t;
//	
//public:
//	UdpConnection(std::shared_ptr<UdpPeer> &p)
//		:_peer(p)
//	{
//	}
//	
//	UdpConnection(unsigned short port)
//	{
//		_peer = std::shared_ptr<UdpPeer>( UdpSock::Create(port) );
//	}
//	
//	template<class PACK>
//	bool Write(const PACK &pck)
//	{
//		typename PACK::serializer_t serializer_t;
//		auto fuc = std::bind(serializer_t, pck, std::placeholders::_1);
//		size_t len=0;
//		typename PACK::stream_t stream = fuc(len);
//		return (_peer->Write((byte_t*)stream.c_str(), len, _addr) == len);
//	}
//	
//	template<class PACK>
//	bool Read(PACK &pck)
//	{
//		typename PACK::unserializer_t unserializer_t;
//		auto fuc = std::bind(unserializer_t, std::placeholders::_1);
//		byte_t bytes[UdpPeer::MAX_LEN] = { 0 };
//		if (_peer->Read(bytes, UdpPeer::MAX_LEN, _addr)< 0)
//			return false;
//		
//		pck = std::move( fuc( PACK::stream_t((char*)bytes) ) );
//		return true;
//	}
//	
//	void Close(){ _peer->Close(); }
//	
//private:
//	std::shared_ptr<UdpPeer> _peer;
//	typedef UdpPeer::addr_t addr_t;
//	addr_t _addr;
//};
	

//template<class PACK, class CONNECTION>
//class Channel
//{
//public:
//	typedef Channel<PACK, CONNECTION> my_t;
//	typedef PACK pack_t;
//	typedef SockConfig config_t;
//	typename pack_t::unserializer_t unserializer_t;
//	typedef CONNECTION conn_t;
//	
//public:
//	Channel() :_conn(nullptr){}
//	Channel(conn_t *conn):_conn(conn){}
//	
//	//void BindIn(INPUT_EVENT &ine)
//	//{
//	//	_input_e = ine;
//	//}
//	//
//	//void BindOut(INPUT_EVENT &ine)
//	//{
//	//	_output_e = ine;
//	//}
//	void Close()
//	{
//		_conn->Close();
//	}
//	
//	void Input()
//	{
//		pack_t pack;
//		_conn->Read(pack);
//		_packs.push( std::move(pack) );
//	}
//	
//	void Output()
//	{
//	}
//
//	const pack_t& Head()const{ return _packs.front(); };
//	void Pop(){ return _packs.pop(); }
//	
//	template<typename INPUT_EVENT>
//	void Input(INPUT_EVENT &_input_e, pack_t &pck)
//	{
//		_input_e(pck);
//	}
//	
//	template<typename OUTPUT_EVENT>
//	void Output(OUTPUT_EVENT &_output_e, const pack_t &src_pck) throw(std::exception)
//	{
//		try{
//			std::vector<pack_t> pcks = _output_e(src_pck);
//			for(auto &i:pcks)
//				_conn->Write(i);
//		}
//		catch(std::exception &e){
//			throw e;
//		}		
//	}	
//		
//private:
//	std::queue<pack_t> _packs;
//	conn_t *_conn;	
//};

template<class PACK>
class UdpChannel
{
public:
	typedef UdpChannel<PACK> my_t;
	typedef PACK pack_t;
	typedef SockConfig conf_t;
	
public:
	
	void Init(conf_t &conf)
	{
		_udp = std::shared_ptr<UdpPeer>(UdpSock::Create(conf.lport));
	}
	
	void Close()
	{
		_udp->Close();
	}

	void Read(pack_t &pck)
	{
		UdpPeer::byte_t buf[1024];
		_udp->Read(buf, 1024, _remote_addr);
		typename pack_t::unserial_t unserializer_t;
		auto fuc = std::bind(unserializer_t, std::placeholders::_1);
		pck = std::move( fuc( pack_t::stream_t(buf) ) );
	}

	void Write(pack_t &pck)
	{
		typename pack_t::serial_t serializer_t;
		auto fuc = std::bind(serializer_t, pck, std::placeholders::_1);
		size_t len = 0;
		typename pack_t::stream_t stream = fuc(len);
		_udp->Write(stream.c_str(), len, _remote_addr);
	}

private:
	std::shared_ptr<UdpPeer> _udp;
	AddrPair _remote_addr;
};


class DefaultWaiter
{
	enum{ INPUT_EVENT, OUTPUT_EVENT };
	int Wait(){ return 0; }
};

//class UdpWaiter
//{
//public:
//	typedef Channel<Jpack, UdpConnection > chn_t;
//	typedef std::shared_ptr<chn_t> chn_ptr_t;
//	typedef std::hash_map<unsigned long, chn_ptr_t > chn_map_t;
//	
//	enum{ INPUT_EVENT, OUTPUT_EVENT, CLOSE_EVENT, ACCEPT_EVENT };
//	
//	int Wait(chn_map_t &chns, chn_ptr_t &chn)
//	{
//		chn_map_t::iterator i = chns.begin();
//		for(; i!=chns.end(); ++i)
//		{
//			chn = i->second;
//			return INPUT_EVENT;
//		}
//		
//		return -1;
//	}
//};

//template<class CHANNEL, class WAITER = DefaultWaiter>
//class ChannelStore
//{
//public:
//	typedef ChannelStore<WAITER, CHANNEL> my_t;
//	typedef SockConfig conf_t;
//	typedef CHANNEL  chn_t;
//	typedef std::shared_ptr<chn_t> chn_ptr_t;
//	typedef typename WAITER::chn_map_t storage_t;
//	
//public:
//	ChannelStore(){}
//	
//	void Store(const config_t &conf)
//	{
//		chn_ptr_t chn(new chn_t(new chn_t::conn_t(conf.lport)));
//		_storage[conf.Hash()] = chn;
//	}
//	
//	chn_ptr_t Drop(const config_t &conf)
//	{
//		auto i = _stroage.find( conf.Hash() );
//		if(i == _storage.end() )
//			return nullptr;
//		
//		chn_ptr_t chn = it->second;
//		_storage.erase(it);
//		return std::move( chn );
//	}
//	
//	chn_ptr_t Get(const config_t &conf)
//	{
//		auto i = _stroage.find( conf.Hash() );
//		if(i == _storage.end() )
//			return nullptr;
//		
//		return std::move( it->second );
//	}
//	
//	//reacted wait method.
//	bool Opening(chn_ptr_t &pchn) throw(std::exception)
//	{
//		switch( _waiter.Wait(_storage, pchn) )
//		{
//			case WAITER::ACCEPT_EVENT: break;
//			case WAITER::INPUT_EVENT:  pchn->Input(); break;
//			case WAITER::OUTPUT_EVENT: pchn->Output();break;
//			case WAITER::CLOSE_EVENT:  pchn->Close(); return false; break;
//			default :throw std::exception("wait io error");
//		}
//		
//		return true;
//	}
//	
//	//synchronous wait method.
//	//bool Wait(chn_ptr_t &chn) throw(std::exception)
//	//{
//	//	for(auto &i:_storage)
//	//	{
//	//		chn = i.second;
//	//		return true;
//	//	}
//	//	
//	//	chn = nullptr;
//	//	return false;
//	//}
//	
//private:
//
//	WAITER _waiter;
//	storage_t _storage;
//
//};


//void NullLoop(Pack &pack){}

//typedef UdpChannel<Pack> udpchn_t;
//typedef ChannelStore<udpchannel_t, UdpWaiter> udpchannelstore_t;

NAMESP_END

#endif /*CHANNEL_H*/
