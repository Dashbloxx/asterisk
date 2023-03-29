#include "stdarg.h"
#include "syscall.h"
#include "syscalltable.h"
#include "string.h"

/*
 *  This file is to be as much independent as possible (no other headers included). For now though, this isn't a thing.
 */

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

int close(int file)
{
    return syscall1(SYS_close, file);
}

int execve(char *name, char **argv, char **env)
{
    return syscall3(SYS_execve, name, argv, env);
}

int fork()
{
    return syscall0(SYS_fork);
}

int fstat(int fd, struct stat *buf)
{
    return syscall2(SYS_fstat, fd, buf);
}

int stat(const char *path, struct stat *buf)
{
    return syscall2(SYS_stat, path, buf);
}

int getpid()
{
    return syscall0(SYS_getpid);
}

int kill(int pid, int sig)
{
    return syscall2(SYS_kill, pid, sig);
}

int link(char *old, char *new);

int lseek(int file, int ptr, int dir)
{
    return syscall3(SYS_lseek, file, ptr, dir);
}

int open(const char *name, int flags, ...)
{
    unsigned short mode = 0;

    if (((flags & O_CREAT) == O_CREAT) /*|| ((flags & O_TMPFILE) == O_TMPFILE)*/)
    {
        va_list args;
        va_start(args, flags);
        mode = (unsigned short)(va_arg(args, int));
        va_end(args);
    }
    return syscall3(SYS_open, name, flags, mode);
}

int read(int file, char *ptr, int len)
{
    return syscall3(SYS_read, file, ptr, len);
}

char *sbrk(int incr)
{
    return syscall1(SYS_sbrk, incr);
}

int wait(int *status)
{
    return syscall1(SYS_wait, status);
}

int execute_on_tty(const char *path, char *const argv[], char *const envp[], const char *tty_path)
{
    return syscall4(SYS_execute_on_tty, path, argv, envp, tty_path);
}

int write(int file, char *ptr, int len)
{
    return syscall3(SYS_write, file, ptr, len);
}

int getdents(int fd, char *buf, int nbytes)
{
    return syscall3(SYS_getdents, fd, (int)buf, nbytes);
}

int execute(const char *path, char *const argv[], char *const envp[])
{
    return syscall3(SYS_execute, (int)path, (int)argv, (int)envp);
}

int getthreads(thread_info* threads, unsigned int max_count, unsigned int flags)
{
    return syscall3(SYS_getthreads, threads, max_count, flags);
}

int getprocs(proc_info* procs, unsigned int max_count, unsigned int flags)
{
    return syscall3(SYS_getprocs, procs, max_count, flags);
}

int manage_pipe(const char *pipe_name, int operation, int data)
{
    return syscall3(SYS_manage_pipe, pipe_name, operation, data);
}

void sleep_ms(unsigned int ms)
{
    syscall1(SYS_sleep_ms, ms);
}

int manage_message(int command, message* msg)
{
    return syscall2(SYS_manage_message, command, (int)msg);
}

int read_dir(int fd, void *dirent, int index)
{
    return syscall3(SYS_asterisk_read_dir, fd, dirent, index);
}

unsigned int get_uptime_ms()
{
    return syscall0(SYS_get_uptime_ms);
}