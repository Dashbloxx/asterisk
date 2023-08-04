/*
 *      dP      Asterisk is an operating system written fully in C and Intel-syntax
 *  8b. 88 .d8  assembly. It strives to be POSIX-compliant, and a faster & lightweight
 *   `8b88d8'   alternative to Linux for i386 processors.
 *   .8P88Y8.   
 *  8P' 88 `Y8  
 *      dP      
 *
 *  BSD 2-Clause License
 *  Copyright (c) 2023 Nexuss
 *  All rights reserved.
 */

#include "unistd.h"
#include "sys/stat.h"
#include "syscall.h"

int access(const char *path, int mode) {
    struct stat sb;

    // Get file information
    if (stat(path, &sb) == -1) {
        // File does not exist
        return -1;
    }

    // Check if file has requested permissions
    switch (mode) {
        case F_OK:
            // File exists
            return 0;
        case X_OK:
            // Check execute permission
            return (sb.st_mode & S_IXUSR) ? 0 : -1;
        case W_OK:
            // Check write permission
            return (sb.st_mode & S_IWUSR) ? 0 : -1;
        case R_OK:
            // Check read permission
            return (sb.st_mode & S_IRUSR) ? 0 : -1;
        default:
            // Invalid mode
            return -1;
    }
}