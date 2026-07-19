.text

.globl coro_ctx_swap
coro_ctx_swap:
    leaq (%rsp),%rax    //rsp -> rax
    movq %rax, 104(%rdi)    //rsp
    movq %rbx, 96(%rdi)     //rbx
    movq %rcx, 88(%rdi)     //rcx
    movq %rdx, 80(%rdi)     //rdx
    movq 0(%rax), %rax
    movq %rax, 72(%rdi)     //ret addr
    movq %rsi, 64(%rdi)     //rsi
    movq %rdi, 56(%rdi)     //rdi
    movq %rbp, 48(%rdi)     //rbp
    movq %r8, 40(%rdi)      //r8
    movq %r9, 32(%rdi)      //r9
    movq %r12, 24(%rdi)     //r12
    movq %r13, 16(%rdi)     //r13
    movq %r14, 8(%rdi)      //r14
    movq %r15, (%rdi)       //r15
    xorq %rax, %rax//   return 0

    movq 48(%rsi), %rbp     //rbp
    movq 104(%rsi), %rsp    //rsp
    movq (%rsi), %r15       //r15
    movq 8(%rsi), %r14      //r14
    movq 16(%rsi), %r13     //r13
    movq 24(%rsi), %r12     //r12
    movq 32(%rsi), %r9      //r9
    movq 40(%rsi), %r8      //r8
    movq 56(%rsi), %rdi     //rdi
    movq 80(%rsi), %rdx     //rdx
    movq 88(%rsi), %rcx     //rcx
    movq 96(%rsi), %rbx     //rbx
    leaq 8(%rsp), %rsp      //rsp += 8
    pushq 72(%rsi)     // push ret addr

    movq 64(%rsi), %rsi // rsi
    ret
