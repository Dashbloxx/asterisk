#include "libc/syscall.h"
#include "libc/syscalltable.h"
#include "libc/stdio.h"

void _start(int argc, char* const argv[], char* const envp[]) {
    printf("Hello, world!\nThis is a number: %x\n", 3);
    _exit();
}