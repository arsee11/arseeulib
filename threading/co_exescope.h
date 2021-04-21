#ifndef CO_EXESCOPE_H
#define CO_EXESCOPE_H

#include <coroutine>

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN

template<class Scope>
class CoExeScope
{
	struct Awaitable {
	    Scope* exe;
	    bool await_ready() { return false; }
	    void await_suspend(std::coroutine_handle<> h) {
                if(exe != nullptr ){
                    exe->post([h](){
                        h.resume();}
                    );
                }
                else{
                    h.resume();
                }
	    }
	    void await_resume() {}
	};
public:
    CoExeScope()=default;

    template<class Poller>
    CoExeScope(Poller* p):_exe(p){}

    Awaitable schedule(){ return Awaitable{&_exe}; }

private:
    Scope _exe;
};

NAMESP_END

#endif /*CO_EXESCOPE_H*/
