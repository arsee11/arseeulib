#include <coroutine>
#include <iostream>
#include "exescope.h"

class CoExeScope
{
	struct Awaitable {
	    arsee::ExeScope* exe;
	    ~Awaitable(){ std::cout<<__FUNCTION__<<endl; }
	    bool await_ready() { return false; }
	    void await_suspend(std::coroutine_handle<> h) {
                if(exe != nullptr ){
                    exe->post([h](){
                        std::cout << "resume...\n";
                        h.resume();}
                    );
                }
                else{
                    h.resume();
                }
                std::cout<<"retun on "<<__FUNCTION__<<endl;
	    }
	    void await_resume() {}
	};
public:
    Awaitable schedule(){ return Awaitable{&_exe}; }

private:
    arsee::ExeScope _exe;
};

