#include "libc/syscall.h"
#include "libc/syscalltable.h"
#include "libc/stdio.h"

/*
 *  For some reason, when I interact with argc or argv, the process ends unexpectedly... I am trying to fix this currently.
 */
int main(int argc, char **argv) {
    printf("Hello, world!\n");
}