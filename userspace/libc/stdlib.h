#pragma once

#include "stdint.h"

#define NULL ((void *)0)

void *malloc(size_t size);
void free(void *ptr);
void __init_libc();