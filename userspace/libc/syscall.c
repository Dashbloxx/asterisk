#include "stdarg.h"
#include "syscall.h"
#include "syscalltable.h"
#include "string.h"

/*
 *  This file is to be as much independent as possible (no other headers included). For now though, this isn't a thing.
 */

/*
 *  These functions syscall0, syscall1, syscall2, syscall3, syscall4, and syscall5 are useful for easily calling
 *  the kernel's syscalls.
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

/*
 *  These functions are wrappers to the `syscall?` functions.
 */

void _exit()
{
    syscall0(SYS_exit);
}

// can be found in line 79 in `syscalls.c` in kernel subdirectory
int close(int file)
{
    return syscall1(SYS_close, file);
}

// can be found in line 91 in `syscalls.c` in kernel subdirectory
int execve(char *name, char **argv, char **env)
{
    return syscall3(SYS_execve, name, argv, env);
}

// can be found in line 88 in `syscalls.c` in kernel subdirectory
int fork()
{
    return syscall0(SYS_fork);
}

// can be found in line 84 in `syscalls.c` in kernel subdirectory
int fstat(int fd, struct stat *buf)
{
    return syscall2(SYS_fstat, fd, buf);
}

// can be found in line 83 in `syscalls.c` in kernel subdirectory
int stat(const char *path, struct stat *buf)
{
    return syscall2(SYS_stat, path, buf);
}

// can be found in line 89 in `syscalls.c` in kernel subdirectory
int getpid()
{
    return syscall0(SYS_getpid);
}

// can be found in line 93 in `syscalls.c` in kernel subdirectory
int kill(int pid, int sig)
{
    return syscall2(SYS_kill, pid, sig);
}

// not defined
int link(char *old, char *new);

// can be found in line 111 in `syscalls.c` in kernel subdirectory
int unlink(const char *name)
{
    return syscall1(SYS_unlink, name);
}

// can be found in line 121 in `syscalls.c` in kernel subdirectory
int lseek(int file, int ptr, int dir)
{
    return syscall3(SYS_lseek, file, ptr, dir);
}

// can be found in line 78 in `syscalls.c` in kernel subdirectory
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

// can be found in line 80 in `syscalls.c` in kernel subdirectory
int read(int file, char *ptr, int len)
{
    return syscall3(SYS_read, file, ptr, len);
}

// can be found in line 87 in `syscalls.c` in kernel subdirectory
char *sbrk(int incr)
{
    return syscall1(SYS_sbrk, incr);
}

// can be found in line 92 in `syscalls.c` in kernel subdirectory
int wait(int *status)
{
    return syscall1(SYS_wait, status);
}

// can be found in line 81 in `syscalls.c` in kernel subdirectory
int write(int file, char *ptr, int len)
{
    return syscall3(SYS_write, file, ptr, len);
}

// can be found in line 98 in `syscalls.c` in kernel subdirectory
int getdents(int fd, char *buf, int nbytes)
{
    return syscall3(SYS_getdents, fd, (int)buf, nbytes);
}

// can be found in line 90 in `syscalls.c` in kernel subdirectory
int execute(const char *path, char *const argv[], char *const envp[])
{
    return syscall3(SYS_execute, (int)path, (int)argv, (int)envp);
}

// can be found in `syscall_getthreads.c`
int getthreads(thread_info* threads, unsigned int max_count, unsigned int flags)
{
    return syscall3(SYS_getthreads, threads, max_count, flags);
}

// can be found in `syscall_getthreads.c`
int getprocs(proc_info* procs, unsigned int max_count, unsigned int flags)
{
    return syscall3(SYS_getprocs, procs, max_count, flags);
}

// can be found in line 101 in `syscalls.c` in kernel subdirectory
int manage_pipe(const char *pipe_name, int operation, int data)
{
    return syscall3(SYS_manage_pipe, pipe_name, operation, data);
}

// can be found in line 104 in `syscalls.c` in kernel subdirectory
void sleep_ms(unsigned int ms)
{
    syscall1(SYS_sleep_ms, ms);
}

// can be found in line 106 in `syscalls.c` in kernel subdirectory
int manage_message(int command, message* msg)
{
    return syscall2(SYS_manage_message, command, (int)msg);
}

// can be found in line 102 in `syscalls.c` in kernel subdirectory
int read_dir(int fd, void *dirent, int index)
{
    return syscall3(SYS_read_dir, fd, dirent, index);
}

// can be found in line 103 in `syscalls.c` in kernel subdirectory
unsigned int get_uptime_ms()
{
    return syscall0(SYS_get_uptime_ms);
}

// can be found in line 100 in `syscalls.c` in kernel subdirectory
int chdir(const char *path)
{
    return syscall1(SYS_chdir, path);
}

// can be found in line 99 in `syscalls.c` in kernel subdirectory
int getcwd(char *buf, size_t size)
{
    return syscall2(SYS_getcwd, buf, size);
}

// can be found in the kernel's `main.c` file
int execute_on_tty(const char *path, char *const argv[], char *const envp[], const char *tty_path)
{
    return syscall4(SYS_execute_on_tty, path, argv, envp, tty_path);
}

// can be found in line 112 in `syscalls.c` in kernel subdirectory
int ftruncate(int fd, int size)
{
    return syscall2(SYS_ftruncate, fd, size);
}

int posix_openpt(int flags)
{
    return syscall1(SYS_posix_openpt, flags);
}

int ptsname_r(int fd, char *buf, int buflen)
{
    return syscall3(SYS_ptsname_r, fd, buf, buflen);
}

int readv(int fd, const struct iovec *iovs, int iovcnt)
{
    return syscall3(SYS_readv, fd, iovs, iovcnt);
}

int writev(int fd, const struct iovec *iovs, int iovcnt)
{
    return syscall3(SYS_writev, fd, iovs, iovcnt);
}

int set_thread_area(void *p)
{
    return syscall1(SYS_set_thread_area, p);
}

int syscall_set_tid_address(void* p)
{
    return syscall1(SYS_set_tid_address, p);
}

int syscall_exit_group(int status)
{
    return syscall1(SYS_exit_group, status);
}

int syscall_llseek(unsigned int fd, unsigned int offset_high, unsigned int offset_low, long *result, unsigned int whence)
{
    return syscall5(SYS_llseek, fd, offset_high, offset_low, result, whence);
}

int statx(int dirfd, const char *pathname, int flags, unsigned int mask, struct statx *statxbuf)
{
    return syscall5(SYS_statx, dirfd, pathname, flags, mask, statxbuf);
}

int clock_gettime64(int clockid, struct timespec *tp)
{
    return syscall2(SYS_clock_gettime64, clockid, tp);
}

int clock_settime64(int clockid, const struct timespec *tp)
{
    return syscall2(SYS_clock_settime64, clockid, tp);
}

int clock_getres64(int clockid, struct timespec *res)
{
    return syscall2(SYS_clock_getres64, clockid, res);
}

int shmget(int key, size_t size, int flag)
{
    return syscall3(SYS_shmget, key, size, flag);
}

void * shmat(int shmid, const void *shmaddr, int shmflg)
{
    return syscall3(SYS_shmat, shmid, shmaddr, shmflg);
}

int shmdt(const void *shmaddr)
{
    return syscall1(SYS_shmdt, shmaddr);
}

int nanosleep(struct timespec *req, struct timespec *rem)
{
    return syscall2(SYS_nanosleep, req, rem);
}

int mount(const char *source, const char *target, const char *fs_type, unsigned long flags, void *data)
{
    return syscall5(SYS_mount, source, target, fs_type, flags, data);
}

int unmount(const char *target)
{
    return syscall1(SYS_unmount, target);
}

int mkdir(const char *path, uint32_t mode)
{
    return syscall2(SYS_mkdir, path, mode);
}

int rmdir(const char *path)
{
    return syscall1(SYS_rmdir, path);
}