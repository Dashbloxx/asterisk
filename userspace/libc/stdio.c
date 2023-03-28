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