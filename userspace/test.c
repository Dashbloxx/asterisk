#include "libc/syscall.h"
#include "libc/syscalltable.h"
#include "libc/stdio.h"

void _start(int argc, char* const argv[], char* const envp[]) {
    puts("Hello, world!\n");
    _exit();
}