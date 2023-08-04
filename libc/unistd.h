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

#pragma once

#define	STDIN_FILENO 0
#define	STDOUT_FILENO 1
#define	STDERR_FILENO 2

#define X_OK 1 /* Test if a file can be executed */
#define W_OK 2 /* Test if a file can be written to */
#define R_OK 4 /* Test if a file can be read */
#define F_OK 0 /* Test if a file exists */

int access(const char *path, int mode);