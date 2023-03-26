#include "libc/syscall.h"
#include "libc/syscalltable.h"
#include "libc/stdio.h"

int main(int argc, char **argv) {
    printf("Hello, world!\n");
    printf("Arg1 = `%s`\n", argv[1]);
}