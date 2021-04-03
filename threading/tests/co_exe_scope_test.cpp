#include <iostream>
#include "co_exescope.h"

struct task{
  struct promise_type {
    task get_return_object() { return {}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };
};
 
task resuming_on_scope(CoExeScope& scope) {
  std::cout << "Coroutine started on thread: " << std::this_thread::get_id() << '\n';
  co_await scope.schedule();
  // awaiter destroyed here
  std::cout << "Coroutine resumed on thread: " << std::this_thread::get_id() << '\n';
}
 
int main() {
    CoExeScope scope;
    std::cout<<"post the func schecudles in the scope\n"; 
    resuming_on_scope(scope);
    std::cout<<"the func will schecudles in the scope\n"; 
    char ch;
    std::cin>>ch;
    return 0;
}
