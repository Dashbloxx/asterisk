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

#include <stdio.h>

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        fprintf(stderr, "error: irregular amount of arguments\nusage: cat <filename>\n");
        return -1;
    }

    FILE *file = fopen(argv[1], "r");

    for(int i = 0; ; i++)
    {
        char c = fgetc(file);
        if(c == -1)
        {
            return 0;
        }
        putc(c);
    }
    return 0;
}