#pragma once

#include "stdint.h"

#define NULL ((void *)0)

void *malloc(unsigned int size);
void free(void *ptr);
void __init_libc();