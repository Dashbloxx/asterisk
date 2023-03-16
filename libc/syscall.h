#pragma once

typedef	char *caddr_t;
typedef	long clock_t;
typedef	unsigned int mode_t;

#define O_RDONLY 00
#define O_WRONLY 01
#define O_RDWR 02
#define O_CREAT 00000100
#define NULL ( (void *) 0)

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

caddr_t sbrk(int incr);

clock_t times(struct tms *buf);

int unlink(char *name);

int wait(int *status);

int write(int file, char *ptr, int len);

int gettimeofday(struct timeval *p, void *z);

int getdents(int fd, char *buf, int nbytes);

int execute(const char *path, char *const argv[], char *const envp[]);

int executep(const char *filename, char *const argv[], char *const envp[]);

int getWorkingDirectory(char *buf, int size);

int setWorkingDirectory(const char *path);