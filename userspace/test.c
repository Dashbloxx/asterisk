#include "libc/syscall.h"
#include "libc/syscalltable.h"
#include "libc/stdio.h"

#define	USER_STACK 0xF0000000

/*
 *  For some reason, when I interact with argc or argv, the process ends unexpectedly... I am trying to fix this currently.
 */
int main(int argc, char **argv) {
    printf("Hello, world!\n");
    printf("Arg1 = `%s`\n", argv[1]);
}