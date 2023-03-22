#include "syscall.h"
#include "syscalltable.h"
#include "stdio.h"

extern int main(int argc, char* const argv[], char* const envp[]);

void _start(int argc, char* const argv[], char* const envp[]) {
    main(argc, argv, envp);
    _exit();
}