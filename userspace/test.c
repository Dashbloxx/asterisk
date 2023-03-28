#include "libc/syscall.h"
#include "libc/syscalltable.h"
#include "libc/stdio.h"
#include "libc/unistd.h"

int main(int argc, char **argv) {
    char c;
    c = fgetc(stdin);
    while(1) {
        fputc(c, stdout);
    }
}