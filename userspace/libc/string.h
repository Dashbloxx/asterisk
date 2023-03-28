#pragma once

#include "stdint.h"

size_t strlen(const char *s1);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strcpy(char *dst, const char *src);
char *strtok(char *str, const char *delim);
char* strstr(const char* haystack, const char* needle);
char *strncpy(char *dst, const char *src, size_t n);
char *strcat(char *s1, const char *s2);
char *strncat(char *s1, const char *s2, size_t n);
void *memset(void *b, int c, int len);
void itoa(char *buf, int base, int d);
void* memcpy(uint8_t *dest, const uint8_t *src, uint32_t len);