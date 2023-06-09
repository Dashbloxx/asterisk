#pragma once

#include "common.h"

typedef int32_t Spinlock;

void spinlock_init(Spinlock* spinlock);
void spinlock_lock(Spinlock* spinlock);
BOOL spinlock_try_lock(Spinlock* spinlock);
void spinlock_unlock(Spinlock* spinlock);