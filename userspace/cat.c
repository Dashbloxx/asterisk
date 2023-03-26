#include "libc/syscall.h"
#include "libc/syscalltable.h"
#include "libc/stdio.h"
#include "libc/string.h"
#include "libc/unistd.h"
#include "libc/stdint.h"

/*
 *  Let's set the maximum size of the string that is to store the contents of the file that should be printed out to the screen. In the future, this may be modified
 *  so that `cat` can print out the exact contents of a file without a character limit...
 */
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    
}