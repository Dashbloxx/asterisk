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
 
#include "common.h"
#include "timer.h"
#include "isr.h"

IsrFunction g_interrupt_handlers[256];

uint32_t g_isr_count = 0;
uint32_t g_irq_count = 0;

void interrupt_register(uint8_t n, IsrFunction handler)
{
    g_interrupt_handlers[n] = handler;
}

void handle_isr(Registers regs)
{
    //Screen_PrintF("handle_isr interrupt no:%d\n", regs.int_no);

    g_isr_count++;

    uint8_t int_no = regs.interruptNumber & 0xFF;

    if (g_interrupt_handlers[int_no] != 0)
    {
        IsrFunction handler = g_interrupt_handlers[int_no];
        handler(&regs);
    }
    else
    {
        kprintf("unhandled interrupt: %d\n", int_no);
        kprintf("Tick: %d\n", get_system_tick_count());
        PANIC("unhandled interrupt");
    }
}

void handle_irq(Registers regs)
{
    g_irq_count++;
    
    // end of interrupt message
    if (regs.interruptNumber >= 40)
    {
        //slave PIC
        outb(0xA0, 0x20);
    }

    outb(0x20, 0x20);

    //Screen_PrintF("irq: %d\n", regs.int_no);

    if (g_interrupt_handlers[regs.interruptNumber] != 0)
    {
        IsrFunction handler = g_interrupt_handlers[regs.interruptNumber];
        handler(&regs);
    }
    else
    {
        //kprintf("unhandled IRQ: %d\n", regs.interruptNumber);
    }
    
}
