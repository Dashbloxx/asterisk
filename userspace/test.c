#include "libc/syscall.h"
#include "libc/syscalltable.h"

void _start(int argc, char* const argv[], char* const envp[]) {
    while(1) { }
    _exit();
}