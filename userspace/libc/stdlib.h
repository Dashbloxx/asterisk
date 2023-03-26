#pragma once

#include "stdint.h"

#define NULL ((void *)0)

void *_malloc(size_t size);
void _free(void *ptr);