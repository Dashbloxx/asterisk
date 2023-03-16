.section .text
.global _start
.type _start, @function
_start:
    push $0
    push $0
    mov %esp, %ebp
    sub $12, %esp
    push %ecx # envp
    push %ebx # argv
    push %eax # argc
    sub $12, %esp
    push %ebx # argv
    mov %ecx, __environ
    call _init
    call __initProgname
    call __initializeThreads
    add $16, %esp
    call main
    add $4, %esp
    push %eax
    call exit

.size _start, . - _start