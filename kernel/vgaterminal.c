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
 
#include "vgaterminal.h"

#define XGA_HEIGHT 25
#define XGA_WIDTH 80

/* Pointer to memory that represents VGA text-mode in your screen... */
/* Let's make sure to change this once we implement a higher-half kernel... */
static uint8_t * g_video_start = (uint8_t*)0xB8000;

static uint8_t g_color = (XGA_COLOR_BLACK << 4) | XGA_COLOR_LIGHT_GREEN;

static void vgaterminal_refresh_terminal(terminal_t* terminal);
static void vgaterminal_add_character(terminal_t* terminal, uint8_t character);
static void vgaterminal_move_cursor(terminal_t* terminal, uint16_t oldLine, uint16_t oldColumn, uint16_t line, uint16_t column);

void vgaterminal_setup(terminal_t* terminal)
{
    terminal->tty->winsize.ws_row = XGA_HEIGHT;
    terminal->tty->winsize.ws_col = XGA_WIDTH;
    terminal->refresh_function = vgaterminal_refresh_terminal;
    terminal->add_character_function = vgaterminal_add_character;
    terminal->move_cursor_function = vgaterminal_move_cursor;
}

static void vgaterminal_set_cursor_visible(BOOL visible)
{
    uint8_t cursor = inb(0x3d5);

    if (visible)
    {
        cursor &= ~0x20;
    }
    else
    {
        cursor |= 0x20;
    }
    outb(0x3D5, cursor);
}

static void vgaterminal_refresh_terminal(terminal_t* terminal)
{
    memcpy(g_video_start, terminal->buffer, XGA_HEIGHT * XGA_WIDTH * 2);
}

static void vgaterminal_add_character(terminal_t* terminal, uint8_t character)
{
    uint8_t * video = g_video_start + (terminal->current_line * XGA_WIDTH + terminal->current_column) * 2;
    
    *video++ = character;
    *video++ = g_color;
}

static void vgaterminal_move_cursor(terminal_t* terminal, uint16_t oldLine, uint16_t oldColumn, uint16_t line, uint16_t column)
{
    uint16_t cursorLocation = line * XGA_WIDTH + column;
    outb(0x3D4, 14);
    outb(0x3D5, cursorLocation >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cursorLocation);
}