/*
 *      dP      Asterisk is an operating system written fully in C and Intel-syntax
 *  8b. 88 .d8  assembly. It strives to be POSIX-compliant, and a faster & lightweight
 *   `8b88d8'   alternative to Linux for i386 processors.
 *   .8P88Y8.   
 *  8P' 88 `Y8  
 *      dP      
 *
 *  Copyright (c) 2023 Nexuss
 *  All rights reserved.
 */

#pragma once

#define NAME_MAX 255

struct dirent {
    unsigned int d_ino;
    unsigned int d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[NAME_MAX+1];
};

typedef struct {
    int fd;
    struct dirent ent;
} DIR;