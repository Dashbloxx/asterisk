#pragma once

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2

#define BUFSIZE 1024

typedef struct _FILE {
    int fd;
    int flags;
    char *buf;
    size_t buf_size;
    size_t buf_pos;
} FILE;

FILE *fopen(const char *path, const char *mode);
int fclose(FILE *fp);
size_t fread(void *ptr, size_t size, size_t count, FILE *fp);
size_t fwrite(const void *ptr, size_t size, size_t count, FILE *fp);