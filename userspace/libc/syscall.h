#pragma once

#include "limits.h"
#include "time.h"
#include "sys/types.h"
#include "stdlib.h"

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2
#define O_CREAT 100

#define MAX_OPENED_FILES 20
#define PROCESS_NAME_MAX 32

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

int syscall0(int num);

int syscall1(int num, int p1);

int syscall2(int num, int p1, int p2);

int syscall3(int num, int p1, int p2, int p3);

int syscall4(int num, int p1, int p2, int p3, int p4);

int syscall5(int num, int p1, int p2, int p3, int p4, int p5);
 
void _exit();

int close(int file);

int execve(char *name, char **argv, char **env);

int fork();

//int fstat(int fd, struct stat *buf);

//int stat(const char *path, struct stat *buf);

int getpid();

int isatty(int file);

int kill(int pid, int sig);

int link(char *old, char *new);

int lseek(int file, int ptr, int dir);

int open(const char *name, int flags, ...);

int read(int file, char *ptr, int len);

char *sbrk(int incr);

int times(struct tms *buf);

int unlink(char *name);

int wait(int *status);

int execute_on_tty(const char *path, char *const argv[], char *const envp[], const char *tty_path);

int write(int file, char *ptr, int len);

int gettimeofday(struct timeval *p, void *z);

int getdents(int fd, char *buf, int nbytes);

int execute(const char *path, char *const argv[], char *const envp[]);

int getthreads(thread_info* threads, unsigned int max_count, unsigned int flags);

int getprocs(proc_info* procs, unsigned int max_count, unsigned int flags);

int manage_pipe(const char *pipe_name, int operation, int data);

void sleep_ms(unsigned int ms);

int manage_message(int command, message* msg);

int soso_read_dir(int fd, void *dirent, int index);

unsigned int get_uptime_ms();