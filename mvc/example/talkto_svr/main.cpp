
#pragma comment(lib, "E:\\lib\\mylib\\mvc\\bin\\mvclibd")
#pragma comment(lib, "E:\\lib\\mylib\\mvc\\bin\\jsonlibd")

#include "server.h"


int main(int argc, char **argv)
{
	member_obj_t mobj;
	member_list_obj_t mlobj;
	msg_obj_t msg_obj;
	msg_list_t msgl_obj;

	objects_t::Init(&mlobj, &mlobj);

	server_t svr;

	udpchannel_t::config_t conf = { 11111, 0, "127.0.0.1", ""};

	UdpSock::Init();
	try{
		svr.Run(conf);
	}
	catch (exception &e){
		cout << e.what() << endl;
	}

	UdpSock::UnInit();
	return 0;

}