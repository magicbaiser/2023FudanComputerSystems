#pragma once

#include <stdlib.h>

namespace coro {

using func_t = void (*)(void *);

struct context {
    void *regs[14];
    int ss_size;
    void* ss_sp;
};

void ctx_make(context *ctx, func_t coro_func, const void *arg);

}  // namespace coro
