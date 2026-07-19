#include "coro.h"

namespace coro {

    static coroutine_env g_coro_env;

    coroutine* create(func_t func, void* args) 
	{
        coroutine* latecoro = new coroutine(func, args);
        latecoro->corophase = 1;
        latecoro->ctx.uc_stack.ss_sp = latecoro->corostack;
        latecoro->ctx.uc_stack.ss_size = maxsize;
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

    static void func_wrap(coroutine* co) 
	{
        if (co->coro_func) 
		{
            co->coro_func(co->args);
        }
        co->end = true;
        co->corophase = 3;
        yield(-1);
    }

    int resume(coroutine* co, int param) 
	{
        g_coro_env.push(co);  												
        co->midnum = param;  												
        switch (co->corophase) 
		{
        	case 1:
            	getcontext(&co->ctx);
            	co->ctx.uc_link = &g_coro_env.main_coro;  								
            	co->corophase = 2;
            	makecontext(&co->ctx, (void (*)(void)) func_wrap, 1, co);  						
            	swapcontext(&g_coro_env.main_coro, &co->ctx);  							
				break;
        	case 2:
            	swapcontext(&g_coro_env.main_coro, &co->ctx);
            	break;
        }
        return co->midnum;
    }

    int yield(int ret) 
	{
        coroutine* co = g_coro_env.pop();
        co->midnum = ret;
        if (ret != -1)
		{
            swapcontext(&co->ctx, &g_coro_env.main_coro);  					
            return co->midnum;
        }
        return -1; 
    }

}  // namespace coro
