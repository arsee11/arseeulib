#if defined(_MSC_VER)
#pragma comment(lib, "../../bin/mvclibd")
#pragma comment(lib, "../../bin/jsonlibd")
#endif

#include "server.h"
#include "logics.h"
#include "controls.h"
#include "mysession.h"

class Preactor{};

template<class Session>
class Acceptor{};

int main(int argc, char **argv)
{
	member_obj_t mobj;
	member_list_obj_t mlobj;
	msg_obj_t msg_obj;
	msg_list_t msgl_obj;

	UdpSock::Init();
	objects_t::Init(&mlobj);

	udpchn_t::conf_t conf = { 11111, 0, "127.0.0.1", "" };
	arsee::UdpServer<arsee::udpchn_t, objects_t, member_login_dispth, member_list_dispth> svr(conf);

	PreactorServer < Preactor, 
		Acceptor<
			MySession<objects_t, member_login_dispth, member_list_dispth> 
		> 
	> 
	psvr(1024, conf);

	try{
		while (true)
			svr.Run();
	}
	catch (exception &e){
		cout << e.what() << endl;
	}

	UdpSock::UnInit();
	return 0;

}

