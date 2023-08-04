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
 
#pragma once

#include "common.h"
#include "fs.h"
#include "fifobuffer.h"
#include "termios.h"
#include "ttydev.h"

typedef struct terminal_t terminal_t;

typedef void (*TerminalRefresh)(terminal_t* terminal);
typedef void (*TerminalAddCharacter)(terminal_t* terminal, uint8_t character);
typedef void (*TerminalMoveCursor)(terminal_t* terminal, uint16_t oldLine, uint16_t oldColumn, uint16_t line, uint16_t column);

typedef struct terminal_t
{
    TtyDev* tty;
    uint8_t* buffer;
    uint16_t current_line;
    uint16_t current_column;
    uint8_t color;
    File* opened_master;
    BOOL disabled;
    TerminalRefresh refresh_function;
    TerminalAddCharacter add_character_function;
    TerminalMoveCursor move_cursor_function;
} terminal_t;



terminal_t* terminal_create(TtyDev* tty, BOOL graphic_mode);
void terminal_destroy(terminal_t* terminal);

void terminal_print(terminal_t* terminal, int row, int column, const char* text);
void terminal_clear(terminal_t* terminal);
void terminal_put_character(terminal_t* terminal, uint8_t c);
void terminal_put_text(terminal_t* terminal, const uint8_t* text, uint32_t size);
void terminal_move_cursor(terminal_t* terminal, uint16_t line, uint16_t column);
void terminal_scroll_up(terminal_t* terminal);

void terminal_send_key(terminal_t* terminal, uint8_t modifier, uint8_t character);