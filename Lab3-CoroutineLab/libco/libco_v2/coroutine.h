#pragma once

#include <cassert>
#include <cstdlib>
#include "coro_ctx.h"

namespace coro {

struct coroutine;
struct coroutine_attr;

coroutine *create(func_t coro_func, void *arg, const coroutine_attr *attr = nullptr);
void release(coroutine *co);

int resume(coroutine *co, int param = 0);
int yield(int ret = 0);

struct stack_mem { 
    int stack_size = 0;           // 栈的大小
    coroutine *owner = nullptr;  
    char *buf = nullptr; 
    char *bp = nullptr;     

    stack_mem(size_t size) : stack_size(size) {
        buf = new char[size];
        bp = buf + size;
    }

    ~stack_mem() {
        delete[] buf;
    }
};

struct share_stack {
    int nextidx = 0;
    int count = 0;
    int stack_size = 0;
    stack_mem **stack_array = nullptr;

    share_stack(int count, size_t stack_size)
        : count(count), stack_size(stack_size) {
        stack_array = new stack_mem*[count];
        for (int i = 0; i < count; i++) 
		{
            stack_array[i] = new stack_mem(stack_size);
        }
    }

    ~share_stack() {
        for (int i = 0; i < count; i++) 
		{
            delete stack_array[i];
        }
        delete[] stack_array;
    }

    stack_mem *get_stackmem() {
        int idx = nextidx % count;
        nextidx++;
        return stack_array[idx];
    }
};

struct coroutine {
    bool started = false;
    bool end = false;

    func_t coro_func = nullptr;
    void *arg = nullptr;

    int midnum = 0;
    
    context ctx = {0};
    
    stack_mem* coro_stack_mem = nullptr;
    char* sp = nullptr;
    int usedsize = 0;
    char* usedbuf = nullptr;
    int stack_size = 0;

    coroutine(func_t func, void* arg) : coro_func(func), arg(arg) {}
    ~coroutine() {}
};

struct coroutine_attr {
    int stack_size = 128 * 1024;
    share_stack *sstack = nullptr;
};

class coroutine_env {
private:
    coroutine* coroall[128];
public:
    coroutine* latecoro = nullptr;
    coroutine* nowcoro = nullptr;
    int midnum = 0;
	int depth = 0;
    coroutine_env() 
	{
        coroutine* main_coro = create(nullptr, nullptr, nullptr);
        push(main_coro);
    }

    coroutine *get_coro(int idx) 
	{
        return coroall[idx];
    }

    void pop() 
	{
        depth--;
    }

    void push(coroutine *co) 
	{
        coroall[depth] = co;
        depth++;
    }
};

}  // namespace coro
