//
// Created by firewolf on 08.02.23.
//
#include <coroutine>
#include "functional"
using namespace::std;
struct promise;
struct coroutine : coroutine_handle<promise>
{
    using promise_type = struct promise;
};
struct promise
{
    coroutine get_return_object() { return {coroutine::from_promise(*this)}; }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
};

struct awaitable
{
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<promise> h)
    {
        func();
    }
    void await_resume() { }

    std::function<void()> func;
};