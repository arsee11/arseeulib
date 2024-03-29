//tcp_svr_test.cpp

#include "acceptor.h"
#include "exescope.h"

#include <iostream>
#include <stdio.h>

using namespace arsee::net;
using namespace arsee;
using namespace std;
using namespace std::placeholders;

struct MySession  
{
    using netpeer_ptr = netpeer_ptr4;

	MySession()
	{
	}

	void onAccept(netpeer_ptr peer)
	{
		cout<<"onAccept["<<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"]"<<endl;
		peer->listenOnRecv(std::bind(&MySession::onInput, this, _1, _2));
		_peers.push_back(peer);
	}

	void onInput(const netpeer_ptr& peer, const MsgSt& msg) 
	{
		cout<<"onInput("<<peer->fd()
			<<")["<<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"]"<<endl;
		cout<<msg.rd_ptr()<<"("<<msg.size()<<")"<<endl;

		MsgSt msg_w(msg);
		peer->write_async(msg_w, [this, peer](int nsend, int err){
				cout<<"have sent to"
					<<"["<<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"] bytes:"
					<<nsend<<endl;
		});
	}

	void onClose(const netpeer_ptr& peer)
	{
		cout<<"onClose["<<peer->remote_addr().ip<<":"<<peer->remote_addr().port<<"]"<<endl;
		_peers.remove(peer);
	}

	std::list<netpeer_ptr> _peers;
};

int main()
{
	EventQueueEpoll eq;
	AcceptorC4 a(&eq, 10000);	
	MySession s;
	a.listenOnAccept(std::bind(&MySession::onAccept, &s, _1));

    CurExeScope<EventQueueEpoll> exe(&eq);

    cout<<"begin in thread:"<<std::this_thread::get_id()<<endl; 
    exe.post([&]{
        cout<<"exec in thread:"<<std::this_thread::get_id()<<endl; 
        cout<<"exe..."<<endl; 
        this_thread::sleep_for(1000ms);
        exe.stop();
    });

    exe.run();
}
