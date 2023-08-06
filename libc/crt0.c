/*
 *      dP      Asterisk is an operating system written fully in C and Intel-syntax
 *  8b. 88 .d8  assembly. It strives to be POSIX-compliant, and a faster & lightweight
 *   `8b88d8'   alternative to Linux for i386 processors.
 *   .8P88Y8.   
 *  8P' 88 `Y8  
 *      dP      
 *
 *  Copyright (c) 2023 Nexuss
 *  All rights reserved.
 */

#include <syscall.h>

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((unsigned int)(!(sizeof(x) % sizeof(0[x])))))

#define	USER_OFFSET 0x40000000
#define	USER_EXE_IMAGE 0x200000 //2MB
#define	USER_ARGV_ENV_SIZE 0x10000 //65KB
#define	USER_ARGV_ENV_LOC (USER_OFFSET + (USER_EXE_IMAGE - USER_ARGV_ENV_SIZE))
#define	USER_STACK 0xF0000000
#define	USER_MMAP_START 0x80000000

extern int main(int argc, char* argv[]);

void _exit()
{
    syscall(SYS_exit);
}

void _start() {
    char** const argvenv = (char**)(USER_STACK);
    main(COUNT_OF(argvenv), argvenv);
    _exit();
}