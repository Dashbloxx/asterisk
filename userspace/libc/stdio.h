#pragma once

#include "stdint.h"

#define O_ACCMODE 3
#define O_RDONLY 0
#define O_WRONLY 1
#define O_RDWR 2

#define _IONBF 0
#define SEEK_CUR 1

#define MAX_INT_LEN 20
#define MAX_DOUBLE_LEN 32

/*
 *  The reason why the type is in caps instead of lowercase, is because historically it used to be a macro, and all macros usually are capitalized. The type is keps
 *  in all caps for compatibility reasons.
 */
typedef struct {
    /*
     *  The file descriptor. This integer is unique for each open file.
     */
    int fd;
    /*
     *  Data that has been read or written to the file.
     */
    char *buffer;
    /*
     *  Hold the address of the next character to be read or written.
     */
    char *ptr;
    /*
     *  "This is an integer representing the number of characters left in the buffer"
     *      -ChatGPT
     */
    int buffer_size;
    /*
     *  "This is an integer representing the buffering mode. There are three possible values: _IONBF (no buffering), _IOLBF (line buffering), or _IOFBF (full
     *  buffering)."
     *      -ChatGPT
     */
    int buffer_mode;
    /*
     *  This is the mode in which we're reading, writing, or something else.
     */
    int mode;
    /*
     *  Contains an error code, if an error happened during the process of opening, closing, writing, or reading of a file.
     */
    int error;
} FILE;

/*
 *  These files represent the current TTY that the process is running in. Useful for just printing to the terminal, printing errors to the terminal, and recieving
 *  user input aswell.
 */
extern FILE *stderr;
extern FILE *stdin;
extern FILE *stdout;

FILE *fopen(const char *filename, const char *mode);
int fflush(FILE *stream);
int fclose(FILE *stream);
int _fprintf(FILE *stream, const char *format, ...);