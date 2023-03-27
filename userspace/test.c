#include "libc/syscall.h"
#include "libc/syscalltable.h"
#include "libc/stdio.h"
#include "libc/unistd.h"

int main(int argc, char **argv) {
    /*
     *  Remember that if you don't allocate it, the kernel will have an error.
     */
    // char a = 'a';
    // write(stdout->fd, &a, 1);
    _fprintf(stdout, "Hello, world!\n");
}