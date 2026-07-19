#pragma once
#include <ucontext.h>
#include <cassert>
#include <iostream>

namespace coro {
    class coroutine;
    using func_t = void (*)(void*);

    coroutine* create(func_t func, void* args);
    void release(coroutine* co);
    int resume(coroutine* co, int param = 0);
    int yield(int ret = 0);
    const int maxsize = 999999;  														

    struct coroutine {
        bool started = false;  														
        bool end = false;  															

        func_t coro_func = nullptr;  												
        void* args = nullptr; 													  

        int midnum = 0;

        ucontext_t ctx = { 0 };
        int corophase;
        char corostack[maxsize];
        coroutine(func_t func, void* args) : coro_func(func), args(args) {}
        ~coroutine() {}
    };

    class coroutine_env {  
    private:
    	int depth = 0; 
        coroutine* coroall[2]; 
    public:
    	int midnum = 0;  											
        ucontext_t main_coro;  										
        coroutine_env() {}

        coroutine* get_coro(int idx) 
		{
            assert(idx == 0 || idx == 1);
            return coroall[idx];
        }
        
        void push(coroutine* co) 
		{
            assert(depth < 2);
            coroall[depth] = co;
            depth++;
        }
        
        coroutine* pop() 
		{
            assert(depth >= 1);
            coroutine* co = coroall[depth - 1];
            depth--;
            return co;
        }
    };

}  // namespace coro
