/*
 *      dP      Asterisk is an operating system written fully in C and Intel-syntax
 *  8b. 88 .d8  assembly. It strives to be POSIX-compliant, and a faster & lightweight
 *   `8b88d8'   alternative to Linux for i386 processors.
 *   .8P88Y8.   
 *  8P' 88 `Y8  
 *      dP      
 *
 *  BSD 2-Clause License
 *  Copyright (c) 2023 Nexuss
 *  All rights reserved.
 */

#include "string.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "sys/types.h"

size_t strlen(const char *s1) {
    size_t i = 0;
    do i++; while (s1[i] != '\0');
    return i;
}

int strcmp(const char *s1, const char *s2) {
    return strncmp(s1, s2, SIZE_MAX);
}

size_t strcspn(const char *s, const char *reject) {
    size_t len = 0;
    char c;
    const char *p;

    while ((c = *s++) != '\0') {
        p = reject;
        while (*p != '\0') {
            if (c == *p++)
                return len;
        }
        len++;
    }
    return len;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    for (size_t pos = 0; pos < n; pos++) {
        char c1 = *s1++;
        char c2 = *s2++;
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
        if (c1 == 0 || c2 == 0) break;
    }
    return 0;
}

char *strcpy(char *dst, const char *src) {
    return strncpy(dst, src, SIZE_MAX);
}

char *strtok(char *str, const char *delim) {
    static char *saved = NULL;

    if (str) {
        saved = str;
    } else if (!saved) {
        return NULL;
    }

    char *start = saved;

    while (*saved) {
        const char *d = delim;
        while (*d) {
            if (*saved == *d) {
                *saved = '\0';
                saved++;
                return start;
            }
            d++;
        }
        saved++;
    }

    if (start == saved) {
        return NULL;
    }

    return start;
}

char* strstr(const char* haystack, const char* needle) {
    if (*needle == '\0') {
        return (char*) haystack;  // empty needle matches any haystack
    }
    
    size_t needle_len = strlen(needle);
    
    for (size_t i = 0; haystack[i] != '\0'; ++i) {
        if (haystack[i] == needle[0]) {
            if (strncmp(&haystack[i], needle, needle_len) == 0) {
                return (char*) &haystack[i];  // match found
            }
        }
    }
    
    return NULL;  // no match found
}

char *strncpy(char *dst, const char *src, size_t n) {
    size_t i = 0;
    do dst[i] = *src; while (*src++ != '\0' && i++ < n);
    return dst;
}

char *strcat(char *s1, const char *s2) {
    return strncat(s1, s2, SIZE_MAX);
}

char *strncat(char *s1, const char *s2, size_t n) {
    strncpy(s1 + strlen(s1), s2, n);
    return s1;
}

void *memset(void *b, int c, int len) {
    int i;
    unsigned char *p = b;
    i = 0;
    while(len > 0)
    {
        *p = c;
        p++;
        len--;
    }
    return(b);
}

void itoa(char *buf, int base, int d) {
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;

    /* If %d is specified and D is minus, put ‘-’ in the head. */
    if (base == 'd' && d < 0) {
        *p++ = '-';
        buf++;
        ud = -d;
    } else if (base == 'x')
        divisor = 16;

    /* Divide UD by DIVISOR until UD == 0. */
    do {
        int remainder = ud % divisor;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while (ud /= divisor);

    /* Terminate BUF. */
    *p = 0;

    /* Reverse BUF. */
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

void* memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
    const uint8_t *sp = (const uint8_t *)src;
    uint8_t *dp = (uint8_t *)dest;
    for(; len != 0; len--) *dp++ = *sp++;

    return dest;
}

int strtol(const char *nptr, char **endptr, int base) {
    /* Skip any leading whitespace */
    while (isspace(*nptr)) {
        ++nptr;
    }

    /* Check for the sign */
    int sign = 1;
    if (*nptr == '+') {
        ++nptr;
    } else if (*nptr == '-') {
        sign = -1;
        ++nptr;
    }

    /* Determine the base */
    if (base == 0) {
        /* Auto-detect the base */
        if (*nptr == '0') {
            ++nptr;
            if (*nptr == 'x' || *nptr == 'X') {
                base = 16;
                ++nptr;
            } else {
                base = 8;
            }
        } else {
            base = 10;
        }
    } else if (base == 16) {
        /* Skip the "0x" or "0X" prefix */
        if (*nptr == '0' && (*(nptr + 1) == 'x' || *(nptr + 1) == 'X')) {
            nptr += 2;
        }
    }

    /* Parse the digits */
    int value = 0;
    while (isalnum(*nptr)) {
        int digit;
        if (isdigit(*nptr)) {
            digit = *nptr - '0';
        } else if (isupper(*nptr)) {
            digit = *nptr - 'A' + 10;
        } else {
            digit = *nptr - 'a' + 10;
        }
        if (digit >= base) {
            /* Invalid digit */
            break;
        }
        value = value * base + digit;
        ++nptr;
    }

    /* Update the end pointer */
    if (endptr != NULL) {
        *endptr = (char *)nptr;
    }

    /* Check for errors, this will be uncommented once errno is implemented... */
    // if (errno != 0) {
    //     return 0;
    // }
    // if (value > LONG_MAX) {
    //     errno = ERANGE;
    //     return sign == 1 ? LONG_MAX : LONG_MIN;
    // }

    /* Return the result */
    return sign * value;
}
