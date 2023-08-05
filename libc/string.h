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
#include "sys/types.h"

size_t strlen(const char *s1);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
size_t strcspn(const char *s, const char *reject);
char *strcpy(char *dst, const char *src);
char *strtok(char *str, const char *delim);
char* strstr(const char* haystack, const char* needle);
char *strncpy(char *dst, const char *src, size_t n);
char *strcat(char *s1, const char *s2);
char *strncat(char *s1, const char *s2, size_t n);
void *memset(void *b, int c, int len);
void itoa(char *buf, int base, int d);
void* memcpy(uint8_t *dest, const uint8_t *src, uint32_t len);
int strtol(const char *nptr, char **endptr, int base);