#include "../libc/syscall.h"
#include "../kernel/syscalltable.h"

int syscall0(int num)
{
    int a;
    asm volatile("int $0x80" : "=a" (a) : "0" (num));
    return a;
}

int syscall1(int num, int p1)
{
    int a;
    asm volatile("int $0x80" : "=a" (a) : "0" (num), "b" ((int)p1));
    return a;
}

int syscall2(int num, int p1, int p2)
{
    int a;
    asm volatile("int $0x80" : "=a" (a) : "0" (num), "b" ((int)p1), "c" ((int)p2));
    return a;
}

int syscall3(int num, int p1, int p2, int p3)
{
    int a;
    asm volatile("int $0x80" : "=a" (a) : "0" (num), "b" ((int)p1), "c" ((int)p2), "d"((int)p3));
    return a;
}

int syscall4(int num, int p1, int p2, int p3, int p4)
{
    int a;
    asm volatile("int $0x80" : "=a" (a) : "0" (num), "b" ((int)p1), "c" ((int)p2), "d" ((int)p3), "S" ((int)p4));
    return a;
}

int syscall5(int num, int p1, int p2, int p3, int p4, int p5)
{
    int a;
    asm volatile("int $0x80" : "=a" (a) : "0" (num), "b" ((int)p1), "c" ((int)p2), "d" ((int)p3), "S" ((int)p4), "D" ((int)p5));
    return a;
}

void _exit()
{
    syscall0(SYS_exit);
}

void _start(int argc, char* const argv[], char* const envp[]) {
    while(1) { }
    _exit();
}