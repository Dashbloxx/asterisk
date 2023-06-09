#include "syscall.h"
#include "syscalltable.h"
#include "stdio.h"

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((unsigned int)(!(sizeof(x) % sizeof(0[x])))))

#define	USER_OFFSET 0x40000000
#define	USER_EXE_IMAGE 0x200000 //2MB
#define	USER_ARGV_ENV_SIZE 0x10000 //65KB
#define	USER_ARGV_ENV_LOC (USER_OFFSET + (USER_EXE_IMAGE - USER_ARGV_ENV_SIZE))
#define	USER_STACK 0xF0000000
#define	USER_MMAP_START 0x80000000

extern int main(int argc, char* argv[]);

void _start() {
    char** const argvenv = (char**)(USER_STACK);
    __init_libc();
    main(COUNT_OF(argvenv), argvenv);
    _exit();
}