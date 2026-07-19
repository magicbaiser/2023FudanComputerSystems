#include "coroutine.h"

#include <cstring>

namespace coro {

static coroutine_env g_coro_env;

extern "C" {
extern void coro_ctx_swap(context*, context*) asm("coro_ctx_swap");
};

coroutine* create(func_t coro_func, void* arg, const coroutine_attr* attr) {
    coroutine_attr at;
    if (attr != nullptr) {
        at = *attr;
    }

    if (at.stack_size < 8 * 1024) 
	{
        at.stack_size = 8 * 1024;
    }  
	if (at.stack_size > 8 * 1024 * 1024) 
	{
        at.stack_size = 8 * 1024 * 1024;
    }
    if (at.stack_size & 0xFFF == 1) 
	{
        at.stack_size = at.stack_size & ~0xFFF;
        at.stack_size = at.stack_size + 0x1000;
    }
    coroutine* latecoro = new coroutine(coro_func, arg);
    stack_mem *latestack_mem = nullptr;
    if (at.sstack == nullptr) 
	{
        latestack_mem = new coro::stack_mem(at.stack_size);
        latestack_mem->owner = latecoro;
    } 
	else 
	{
        latestack_mem = at.sstack->get_stackmem();
    }
    latecoro->coro_stack_mem = latestack_mem;
    latecoro->stack_size = latecoro->coro_stack_mem->stack_size;
    latecoro->ctx.ss_size = latecoro->stack_size;
    latecoro->ctx.ss_sp = latecoro->coro_stack_mem->buf;
    return latecoro;
}

void release(coroutine* co) 
{
    if (co != 0) 
	{
		delete co;	
	}
    return;
}

void save_stack(coroutine* co) {
    stack_mem* latestack_mem = co->coro_stack_mem;
    int len = latestack_mem->bp - co->sp;
    if (co->usedbuf)
    {
        delete[] co->usedbuf;
        co->usedbuf = nullptr;
    }
    co->usedbuf = new char[len];
    co->usedsize = len;
    memcpy(co->usedbuf, co->sp, len);
}

void swap(coroutine* curr, coroutine* pending) {
    char c;
    curr->sp = &c;
    g_coro_env.latecoro = pending;
    g_coro_env.nowcoro  = pending->coro_stack_mem->owner;
    pending->coro_stack_mem->owner = pending;
    if (g_coro_env.nowcoro  && g_coro_env.nowcoro != pending)
    {
        save_stack(g_coro_env.nowcoro);
    }
    coro_ctx_swap(&(curr->ctx), &(pending->ctx));
    if (g_coro_env.nowcoro && g_coro_env.latecoro && g_coro_env.latecoro != g_coro_env.nowcoro)
    {
        if (g_coro_env.latecoro->usedbuf && g_coro_env.latecoro->usedsize > 0)
        {
            memcpy(g_coro_env.latecoro->sp, g_coro_env.latecoro->usedbuf, g_coro_env.latecoro->usedsize);
        }
    }
}

static void func_wrap(coroutine* co) {
    if (co->coro_func) {
        co->coro_func(co->arg);
    }
    co->end = true;
    yield(-1);
}

int resume(coroutine* co, int param) {
    co->midnum = param;
    if (!co->started) 
	{
        ctx_make(&co->ctx, (func_t)func_wrap, co);
        co->started = true;
    }
    g_coro_env.push(co);
    swap(g_coro_env.get_coro(g_coro_env.depth - 2), co);
    return co->midnum;
}

int yield(int ret) {
    coroutine* curr = g_coro_env.get_coro(g_coro_env.depth - 1);
    g_coro_env.pop();
    curr->midnum = ret;
    swap(curr, g_coro_env.get_coro(g_coro_env.depth - 1));
    return curr->midnum;
}

}  // namespace coro
