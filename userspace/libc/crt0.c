#include "syscall.h"
#include "syscalltable.h"
#include "stdio.h"

#define	USER_OFFSET 0x40000000
#define	USER_EXE_IMAGE 0x200000 //2MB
#define	USER_ARGV_ENV_SIZE 0x10000 //65KB
#define	USER_ARGV_ENV_LOC (USER_OFFSET + (USER_EXE_IMAGE - USER_ARGV_ENV_SIZE))

extern int main(int argc, char* argv[], char* envp[]);

void _start() {
    main(sizeof((char**)USER_ARGV_ENV_LOC) / sizeof(((char**)USER_ARGV_ENV_LOC)[0]), (char**)USER_ARGV_ENV_LOC, 0);
    _exit();
}