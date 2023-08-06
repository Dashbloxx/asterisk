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

#include <unistd.h>
#include <syscall.h>
#include <stdarg.h>

int open(const char *name, int flags, ...)
{
    unsigned short mode = 0;

    if (((flags & O_CREAT) == O_CREAT))
    {
        va_list args;
        va_start(args, flags);
        mode = (unsigned short)(va_arg(args, int));
        va_end(args);
    }
    return syscall(SYS_open, name, flags, mode);
}

int read(int file, char *ptr, int len)
{
    return syscall(SYS_read, file, ptr, len);
}

int write(int file, char *ptr, int len)
{
    return syscall(SYS_write, file, ptr, len);
}

int close(int file)
{
    return syscall(SYS_close, file);
}