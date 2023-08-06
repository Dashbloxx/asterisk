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

#include <stddef.h>
#include <stdint.h>

#define isalpha(a) ((((unsigned)(a)|32)-'a') < 26)
#define isdigit(a) (((unsigned)(a)-'0') < 10)
#define islower(a) (((unsigned)(a)-'a') < 26)
#define isupper(a) (((unsigned)(a)-'A') < 26)
#define isprint(a) (((unsigned)(a)-0x20) < 0x5f)
#define isgraph(a) (((unsigned)(a)-0x21) < 0x5e)
#define isspace(a) (a == ' ' || (unsigned)a-'\t' < 5)
#define iscntrl(a) ((unsigned)a < 0x20 || a == 0x7f)
#define tolower(a) ((a)|0x20)
#define toupper(a) ((a)&0x5f)
#define isalnum(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z') || ((c) >= '0' && (c) <= '9'))


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