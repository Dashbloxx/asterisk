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

#pragma once

#include "limits.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "stdlib.h"

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2
#define O_CREAT 100

#define MAX_OPENED_FILES 20
#define PROCESS_NAME_MAX 32

/*
 *  These structs are taken from the kernel.
 */

typedef struct thread_info
{
    unsigned int thread_id;
    unsigned int process_id;
    unsigned int state;
    unsigned int user_mode;

    unsigned int birth_time;
    unsigned int context_switch_count;
    unsigned int context_start_time;
    unsigned int context_end_time;
    unsigned int consumed_cpu_time_ms;
    unsigned int usage_cpu;
    unsigned int called_syscall_count;
} thread_info;

typedef struct proc_info
{
    unsigned int process_id;
    int parent_process_id;
    unsigned int fd[MAX_OPENED_FILES];

    char name[PROCESS_NAME_MAX];
    char tty[128];
    char working_directory[128];
} proc_info;

typedef struct message
{
    unsigned int message_type;
    unsigned int parameter1;
    unsigned int parameter2;
    unsigned int parameter3;
} message;

struct iovec {
    void  *iov_base;    /* Starting address */
    size_t iov_len;     /* Number of bytes to transfer */
};

struct timespec
{
    time_t tv_sec;        /* seconds */
    unsigned int tv_nsec;       /* nanoseconds */
};

struct statx {
    unsigned int stx_mask;
    unsigned int stx_blksize;
    unsigned long stx_attributes;
    unsigned int stx_nlink;
    unsigned int stx_uid;
    unsigned int stx_gid;
    unsigned short stx_mode;
    unsigned short pad1;
    unsigned long stx_ino;
    unsigned long stx_size;
    unsigned long stx_blocks;
    unsigned long stx_attributes_mask;
    struct {
        long tv_sec;
        unsigned int tv_nsec;
        int pad;
    } stx_atime, stx_btime, stx_ctime, stx_mtime;
    unsigned int stx_rdev_major;
    unsigned int stx_rdev_minor;
    unsigned int stx_dev_major;
    unsigned int stx_dev_minor;
    unsigned long spare[14];
};

/* These functions make it easier to define functions that call syscalls */

int syscall0(int num);

int syscall1(int num, int p1);

int syscall2(int num, int p1, int p2);

int syscall3(int num, int p1, int p2, int p3);

int syscall4(int num, int p1, int p2, int p3, int p4);

int syscall5(int num, int p1, int p2, int p3, int p4, int p5);
 
void _exit();

// can be found in line 79 in `syscalls.c` in kernel subdirectory
int close(int file);

// can be found in line 91 in `syscalls.c` in kernel subdirectory
int execve(char *name, char **argv, char **env);

// can be found in line 88 in `syscalls.c` in kernel subdirectory
int fork();

// can be found in line 84 in `syscalls.c` in kernel subdirectory
int fstat(int fd, struct stat *buf);

// can be found in line 83 in `syscalls.c` in kernel subdirectory
int stat(const char *path, struct stat *buf);

// can be found in line 89 in `syscalls.c` in kernel subdirectory
int getpid();

// can be found in line 93 in `syscalls.c` in kernel subdirectory
int kill(int pid, int sig);

// not defined
int link(char *old, char *new);

// can be found in line 111 in `syscalls.c` in kernel subdirectory
int unlink(const char *name);

// can be found in line 121 in `syscalls.c` in kernel subdirectory
int lseek(int file, int ptr, int dir);

// can be found in line 78 in `syscalls.c` in kernel subdirectory
int open(const char *name, int flags, ...);

// can be found in line 80 in `syscalls.c` in kernel subdirectory
int read(int file, char *ptr, int len);

// can be found in line 87 in `syscalls.c` in kernel subdirectory
char *sbrk(int incr);
// can be found in line 92 in `syscalls.c` in kernel subdirectory
int wait(int *status);

// can be found in line 105 in `syscalls.c` in kernel subdirectory
int execute_on_tty(const char *path, char *const argv[], char *const envp[], const char *tty_path);

// can be found in line 81 in `syscalls.c` in kernel subdirectory
int write(int file, char *ptr, int len);

// can be found in line 98 in `syscalls.c` in kernel subdirectory
int getdents(int fd, char *buf, int nbytes);

// can be found in line 90 in `syscalls.c` in kernel subdirectory
int execute(const char *path, char *const argv[], char *const envp[]);

// can be found in `syscall_getthreads.c`
int getthreads(thread_info* threads, unsigned int max_count, unsigned int flags);

// can be found in `syscall_getthreads.c`
int getprocs(proc_info* procs, unsigned int max_count, unsigned int flags);

// can be found in line 101 in `syscalls.c` in kernel subdirectory
int manage_pipe(const char *pipe_name, int operation, int data);

// can be found in line 104 in `syscalls.c` in kernel subdirectory
void sleep_ms(unsigned int ms);

// can be found in line 106 in `syscalls.c` in kernel subdirectory
int manage_message(int command, message* msg);

// can be found in line 102 in `syscalls.c` in kernel subdirectory
int read_dir(int fd, void *dirent, int index);

// can be found in line 103 in `syscalls.c` in kernel subdirectory
unsigned int get_uptime_ms();

// can be found in line 100 in `syscalls.c` in kernel subdirectory
int chdir(const char *path);

// can be found in line 99 in `syscalls.c` in kernel subdirectory
int getcwd(char *buf, size_t size);

// can be found in line 112 in `syscalls.c` in kernel subdirectory
int ftruncate(int fd, int size);

int posix_openpt(int flags);

int ptsname_r(int fd, char *buf, int buflen);

int readv(int fd, const struct iovec *iovs, int iovcnt);

int writev(int fd, const struct iovec *iovs, int iovcnt);

int set_thread_area(void *p);

int syscall_set_tid_address(void* p);

int syscall_exit_group(int status);

int syscall_llseek(unsigned int fd, unsigned int offset_high, unsigned int offset_low, long *result, unsigned int whence);

int statx(int dirfd, const char *pathname, int flags, unsigned int mask, struct statx *statxbuf);

int clock_gettime64(int clockid, struct timespec *tp);

int clock_settime64(int clockid, const struct timespec *tp);

int clock_getres64(int clockid, struct timespec *res);

int shmget(int key, size_t size, int flag);

void * shmat(int shmid, const void *shmaddr, int shmflg);

int shmdt(const void *shmaddr);

int nanosleep(struct timespec *req, struct timespec *rem);

int mount(const char *source, const char *target, const char *fs_type, unsigned long flags, void *data);

int unmount(const char *target);

int mkdir(const char *path, unsigned int mode);

int rmdir(const char *path);