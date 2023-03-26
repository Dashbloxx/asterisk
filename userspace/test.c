#include "libc/syscall.h"
#include "libc/syscalltable.h"
#include "libc/stdio.h"
#include "libc/unistd.h"

int main(int argc, char **argv) {
    FILE *fp;
    fp = fopen("/dev/com1", O_RDWR);
    write(fp->fd, "Hello, world!\n", strlen("Hello, world!\n"));
    fclose(fp);
    return 0;
}