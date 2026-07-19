#include <coroutine>
#include <functional>
#include <queue>
#include <thread>

namespace coro {

static std::queue<std::function<bool()>> task_queue;

struct sleep {
    sleep(int n_ms) : delay{n_ms} {}

    std::chrono::milliseconds delay;
    bool await_ready() noexcept { return false; }
    void await_suspend(std::coroutine_handle<> h) noexcept 
	{
        task_queue.pop();
        std::this_thread::sleep_for(delay);
        task_queue.push([h]() -> bool 
		{
            h.resume();
            return h.done();
        });
    }
    void await_resume()  noexcept {}
};

struct Task {

    struct promise_type 
	{
		std::suspend_always initial_suspend()  { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		std::suspend_always yield_value(std::function<bool()> func) {
            task_queue.push(func);
            return {};
        }
        void return_void() { return ;}
        Task get_return_object() 
		{
            return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        void unhandled_exception() { std::terminate(); }
    };
    std::coroutine_handle<promise_type> coro;
    
    Task(std::coroutine_handle<promise_type> h) : coro(h) {
        task_queue.push([this]() -> bool 
		{
            coro.resume();
            return coro.done();
        });
    }
    
    ~Task() 
	{
        if (coro) 
		{
            coro.destroy();
        }
    }

};

void wait_task_queue_empty() {
    while (!task_queue.empty()) 
	{
        std::function<bool()> func = task_queue.front();    
        if (func()) 
		{
            task_queue.pop();
        }
    }
}

}  // namespace coro
