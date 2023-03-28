#include "stdio.h"
#include "syscall.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdarg.h"
#include "errno.h"

FILE *stderr;
FILE *stdin;
FILE *stdout;

FILE *fopen(const char *filename, const char *mode) {
    FILE *fp = malloc(sizeof(FILE));
    if(mode == "r") {
        fp->fd = open(filename, O_RDONLY);
    }
    else if(mode = "w") {
        fp->fd = open(filename, O_WRONLY);
    }
    else if(mode == "r+") {
        fp->fd = open(filename, O_RDWR);
    }
    return fp;
}

void fclose(FILE *file) {
    close(file->fd);
}

size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream) {
    int write_output = write(stream->fd, ptr, count);
    return count;
}

void fread(void *ptr, size_t size, size_t count, FILE *stream) {
    read(stream->fd, ptr, count);
}

void fputc(int c, FILE *stream) {
    unsigned char ch = (unsigned char)c;
    int result = write(stream->fd, &ch, 1);
    return ch;
}

int fgetc(FILE *stream) {
    unsigned char c;
    int result = read(stream->fd, &c, 1);

    if (result != 1) {
        return -1;
    }
    
    return c;
}