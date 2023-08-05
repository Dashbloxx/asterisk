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

#define NULL ((void *)0)

void *malloc(unsigned int size);
void free(void *ptr);
void __init_libc();