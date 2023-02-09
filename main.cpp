#include <iostream>
#include <coroutine>
#include <thread>

using namespace::std;


/*struct awaitable
{
    bool await_ready() { return false; }
    void await_suspend(coroutine_handle<> h)
    {
        [h]{h.resume();};
        printf("%d \n", h);
        std::cout << "Taked new address: " << h.address() << '\n'; // this is OK
        return;
    }
    void await_resume() {}
};
awaitable switch_to_new_thread(coroutine_handle<> handle)
{
    return awaitable();
}
struct task
{
    struct promise_type
    {
        task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};



task resuming_on_new_thread(coroutine_handle<> out)
{
    std::cout << "BEFORE - " << out.address() << '\n';
    switch_to_new_thread(out)
    co_await switch_to_new_thread(out);
    // awaiter destroyed here
    std::cout << "AFTER - " << out.address() << "ID - " << std::this_thread::get_id() <<'\n';
    //while(true);
}

int main()
{jthread
    coroutine_handle handle;
    cout << "MAIN THREAD ID - " << handle.address() << endl;
    printf("%d \n", handle);
    resuming_on_new_thread(handle);

    return 0;
}*/

// jthread example
auto switch_to_new_thread(std::jthread& out)
{
    struct awaitable
    {
        std::jthread* p_out;
        bool await_ready() { return false; }
        void await_suspend(std::coroutine_handle<> h)
        {
            std::jthread& out = *p_out;
            if (out.joinable())
                throw std::runtime_error("Output jthread parameter not empty");
            out = std::jthread([h] { h.resume(); });
            // Potential undefined behavior: accessing potentially destroyed *this
            // std::cout << "New thread ID: " << p_out->get_id() << '\n';
            std::cout << "New thread: " << out.get_id() << '\n'; // this is OK
        }
        void await_resume() {}
    };
    return awaitable{&out};
}

struct task
{
    struct promise_type
    {
        task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

task resuming_on_new_thread(std::jthread& out)
{
    std::cout << "BEFORE - " << std::this_thread::get_id() << '\n';
    co_await switch_to_new_thread(out);
    // awaiter destroyed here
    std::cout << "AFTER - " << std::this_thread::get_id() << '\n';
    for(int i = 0; ; i++)
    {
        cout << "123" << endl;
    }
}

int main()
{
    jthread out;
    resuming_on_new_thread(out);
    cout << "MAIN THREAD ID - " << this_thread::get_id() << endl;
    for(int i = 0; ; i++)
    {
        cout << i << endl;
    }
    return 0;
}