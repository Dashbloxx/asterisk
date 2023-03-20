#pragma once

#include "stdint.h"

size_t strlen(const char *s1);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strcpy(char *dst, const char *src);
char *strncpy(char *dst, const char *src, size_t n);
char *strcat(char *s1, const char *s2);
char *strncat(char *s1, const char *s2, size_t n);
void *memset(void *b, int c, int len);
void itoa(int num, char *number);