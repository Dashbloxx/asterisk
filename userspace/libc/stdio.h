#pragma once

#include "stdint.h"

#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2

/*
 *  The reason why the type is in caps instead of lowercase, is because historically it used to be a macro, and all macros usually are capitalized. The type is keps
 *  in all caps for compatibility reasons.
 */
typedef struct file {
    char *buffer;      // pointer to the buffer
    size_t buf_size;   // size of the buffer
    char *buf_ptr;     // pointer to the current position in the buffer
    int file_desc;     // file descriptor for the open file
    int mode;          // mode in which the file is opened
    int ungetc_char;   // character to unget, if any
    int is_eof;        // flag indicating if end-of-file has been reached
} FILE;

void puts(const char *text);
void putc(char character);
char getchar();