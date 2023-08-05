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

typedef unsigned int uint32_t;
typedef unsigned int uintptr_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;

#define SIZE_MAX sizeof(uint32_t)