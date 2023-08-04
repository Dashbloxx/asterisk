/*
 *      dP      Asterisk is an operating system written fully in C and Intel-syntax
 *  8b. 88 .d8  assembly. It strives to be POSIX-compliant, and a faster & lightweight
 *   `8b88d8'   alternative to Linux for i386 processors.
 *   .8P88Y8.   
 *  8P' 88 `Y8  
 *      dP      
 *
 *  BSD 2-Clause License
 *  Copyright (c) 2017, ozkl, Nexuss
 *  
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *  
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#include "timer.h"
#include "isr.h"
#include "process.h"
#include "common.h"

#define TIMER_FREQ 1000

uint64_t g_system_tick_count = 0;

uint64_t g_system_date_ms = 0;

BOOL g_scheduler_enabled = FALSE;

//called from assembly
void handle_timer_irq(TimerInt_Registers registers)
{
    g_system_tick_count++;

    g_system_date_ms++;

    if (g_scheduler_enabled == TRUE)
    {
        schedule(&registers);
    }
}

uint32_t get_system_tick_count()
{
    return (uint32_t)g_system_tick_count;
}

uint64_t get_system_tick_count64()
{
    return g_system_tick_count;
}

uint32_t get_uptime_seconds()
{
    return ((uint32_t)g_system_tick_count) / TIMER_FREQ;
}

uint64_t get_uptime_seconds64()
{
    //TODO: make this real 64 bit
    return get_uptime_seconds();
}

uint32_t get_uptime_milliseconds()
{
    return (uint32_t)g_system_tick_count;
}

uint64_t get_uptime_milliseconds64()
{
    return g_system_tick_count;
}

void scheduler_enable()
{
    g_scheduler_enabled = TRUE;
}

void scheduler_disable()
{
    g_scheduler_enabled = FALSE;
}

static void timer_init(uint32_t frequency)
{
    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);

    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

    outb(0x40, l);
    outb(0x40, h);
}

void timer_initialize()
{
    timer_init(TIMER_FREQ);
}

int32_t clock_getres64(int32_t clockid, struct timespec *res)
{
    res->tv_sec = 0;
    res->tv_nsec = 1000000;

    return 0;
}

int32_t clock_gettime64(int32_t clockid, struct timespec *tp)
{
    //TODO: clockid

    //TODO: make proper use of 64 bit fields

    uint32_t uptime_milli = g_system_date_ms;

    tp->tv_sec = uptime_milli / TIMER_FREQ;

    uint32_t extra_milli = uptime_milli - tp->tv_sec * 1000;

    tp->tv_nsec = extra_milli * 1000000;

    return 0;
}

int32_t clock_settime64(int32_t clockid, const struct timespec *tp)
{
    //TODO: clockid

    //TODO: make use of tv_nsec

    uint32_t uptime_milli = g_system_date_ms;

    g_system_date_ms = tp->tv_sec * TIMER_FREQ;

    return 0;
}