#if defined(_MSC_VER)
#pragma comment(lib, "../../bin/mvclibd")
#pragma comment(lib, "../../bin/jsonlibd")
#endif

#include "server.h"
#include "logics.h"
#include "controls.h"
#include "mysession.h"
#include "net/preactor.h"
#include "net/acceptor.h"
#include "net/fd.h"
#include "net/selector.h"
#include "jpack.h"



typedef UdpChannel<Jpack> udpchn_t;

int main(int argc, char **argv)
{
	member_obj_t mobj;
	member_list_obj_t mlobj;
	msg_obj_t msg_obj;
	msg_list_t msgl_obj;

	SockInit();
	objects_t::Init(&mlobj);

	udpchn_t::conf_t conf = { 11111, 0, "127.0.0.1", "" };
//	arsee::UdpServer<arsee::udpchn_t, objects_t, member_login_dispth, member_list_dispth> svr(conf);

	typedef Preactor<FdHolder, true, Epoll> tcp_preactor_t;
	PreactorServer < tcp_preactor_t, 
		Acceptor<
			MySession<Jpack,objects_t, member_login_dispth, member_list_dispth> 
		> 
	> 
	svr(1024, conf);

	try{
		while (true)
			svr.Run();
	}
	catch (exception &e){
		cout << e.what() << endl;
	}

	SockUninit();
	return 0;

}

