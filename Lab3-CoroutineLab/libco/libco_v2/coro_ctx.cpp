#include "coro_ctx.h"

#include <cstdint>
#include <cstring>

namespace coro {

void ctx_make(context* ctx, func_t coro_func, const void* arg) {
	for(int i = 0 ; i < 14 ; i++)
    {
    	ctx->regs[i] = 0;
	}
    char* sp = (char*)ctx->ss_sp + ctx->ss_size - sizeof(void*);
    sp = (char*)((uintptr_t)sp & -16LL); 
    ctx->regs[7] = (void*) arg;
    ctx->regs[9] = (void*) coro_func;
    ctx->regs[13] = sp;
}

}  // namespace coro
