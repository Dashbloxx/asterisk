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

#include "stdint.h"

#define O_ACCMODE 3
#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2

/*
 *  This struct contains all the values that you'll usually find in the `FILE` struct type. Well, the type is actually `file_t`, but
 *  then theres a macro that defines `FILE` as `file_t`...
 */
typedef struct {
    int fd; /* Contains a unique integer that represents the file. Usually this integer can be returned by the `open` syscall... */
    int mode; /* This contains the mode in which the file is opened... */
    int error; /* This is to contain an error code, if an error happens... */
    unsigned char *buffer; /* The buffer is stored in RAM, and is to have the file's contents replicated into this buffer... */
    unsigned int buffer_size; /* This contains the size that the buffer should be. */
    unsigned int position; /* The current position in file, whether it's being read or written to... */
} file_t;

/* Let's define `FILE` the right way. If we want it capitalized, let's use a macro to do it! */
#define FILE file_t

/*
 *  These files represent the current TTY that the process is running in. Useful for just printing to the terminal, printing errors to the terminal, and recieving
 *  user input aswell.
 */
extern FILE *stderr;
extern FILE *stdin;
extern FILE *stdout;

/*
 *  Functions related to file I/O...
 */

/* Open a file and return a FILE pointer... */
FILE *fopen(const char *filename, const char *mode);

/* Close a file by it's `FILE` struct... */
int fclose(FILE *file);

/* Write to a file by it's `FILE` struct... */
unsigned int fwrite(const void *ptr, unsigned int size, unsigned int count, FILE *stream);

/* Read from file using `FILE` type... */
unsigned int fread(void *ptr, unsigned int size, unsigned int count, FILE *stream);

int fgetc(FILE* stream);

/* Flush output... */
int fflush(FILE *stream);

/* Print formatted text to a file... */
int fprintf(FILE *stream, const char *format, ...);

/*
 *  Functions related to terminal I/O...
 */

int putc(int c);
int getc();
int printf(const char *format, ...);
char *gets(char *s);

/* All printf variations will be implemented without using another printf variation... */
int snprintf(char *str, unsigned int size, const char *format, ...);

#define EOF -1