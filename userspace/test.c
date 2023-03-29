#include "libc/syscall.h"
#include "libc/syscalltable.h"
#include "libc/stdio.h"
#include "libc/string.h"
#include "libc/unistd.h"

/*
 *  This is a horrible shell that isn't even finished yet & is work-in-progress, come back later if you want it working...
 */

int main(int argc, char **argv) {
    /*
     *  The name of the program to execute.
     */
    char program[1024];
    /*
     *  The full command; the full text inputted to the shell.
     */
    char command[4096];
    /*
     *  The current directory that the shell is running on.
     */
    char path[4096];
    /* Let's set the current path to the root of the filesystem... */
    strcpy(path, "/");
    while(1) {
        printf("[%s]-$ ", path);
        char *a = fgets(command, 4096, stdin);
        if(a == NULL) {
            continue;
        }
        else {
            char *token = strtok(command, " ");
            strcpy(program, token);

        }
    }
}