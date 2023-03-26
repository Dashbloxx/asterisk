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
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    /* Check that we have one argument */
    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        return 1;
    }

    /* Open the file descriptor */
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "error: could not open file\n");
        return 1;
    }

    /* Read the contents of the file into the buffer */
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        /* Print the buffer */
        if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read) {
            fprintf(stderr, "error: could not write to stdout\n");
            close(fd);
            return 1;
        }
    }

    /* Check for read error */
    if (bytes_read == -1) {
        fprintf(stderr, "error: could not read from file\n");
        close(fd);
        return 1;
    }

    /* Close the file descriptor */
    close(fd);

    return 0;
}