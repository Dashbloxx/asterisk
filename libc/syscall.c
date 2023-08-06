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
#include <stdarg.h>

int syscall(int num, ...)
{
    int a;
    va_list args;
    va_start(args, num);

    switch (num)
    {
    case 0:
        asm volatile("int $0x80"
                     : "=a"(a)
                     : "0"(num));
        break;
    case 1:
        asm volatile("int $0x80"
                     : "=a"(a)
                     : "0"(num), "b"(va_arg(args, int)));
        break;
    case 2:
        asm volatile("int $0x80"
                     : "=a"(a)
                     : "0"(num), "b"(va_arg(args, int)), "c"(va_arg(args, int)));
        break;
    case 3:
        asm volatile("int $0x80"
                     : "=a"(a)
                     : "0"(num), "b"(va_arg(args, int)), "c"(va_arg(args, int)), "d"(va_arg(args, int)));
        break;
    case 4:
        asm volatile("int $0x80"
                     : "=a"(a)
                     : "0"(num), "b"(va_arg(args, int)), "c"(va_arg(args, int)), "d"(va_arg(args, int)), "S"(va_arg(args, int)));
        break;
    case 5:
        asm volatile("int $0x80"
                     : "=a"(a)
                     : "0"(num), "b"(va_arg(args, int)), "c"(va_arg(args, int)), "d"(va_arg(args, int)), "S"(va_arg(args, int)), "D"(va_arg(args, int)));
        break;
    case 6:
        asm volatile("int $0x80"
                     : "=a"(a)
                     : "0"(num), "b"(va_arg(args, int)), "c"(va_arg(args, int)), "d"(va_arg(args, int)), "S"(va_arg(args, int)), "D"(va_arg(args, int)), "m"(va_arg(args, int)));
        break;
    default:
        return -1;
        break;
    }

    va_end(args);
    return a;
}
