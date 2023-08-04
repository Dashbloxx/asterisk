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
 
#include "gfx.h"
#include "fbterminal.h"

static void fbterminal_refreshTerminal(terminal_t* terminal);
static void fbterminal_addCharacter(terminal_t* terminal, uint8_t character);
static void fbterminal_moveCursor(terminal_t* terminal, uint16_t old_line, uint16_t old_column, uint16_t line, uint16_t column);

void fbterminal_setup(terminal_t* terminal)
{
    terminal->tty->winsize.ws_row = gfx_get_height() / 16;
    terminal->tty->winsize.ws_col = gfx_get_width() / 9;
    terminal->refresh_function = fbterminal_refreshTerminal;
    terminal->add_character_function = fbterminal_addCharacter;
    terminal->move_cursor_function = fbterminal_moveCursor;
}

static void fbterminal_refreshTerminal(terminal_t* terminal)
{
    for (uint32_t r = 0; r < terminal->tty->winsize.ws_row; ++r)
    {
        for (uint32_t c = 0; c < terminal->tty->winsize.ws_col; ++c)
        {
            uint8_t* characterPos = terminal->buffer + (r * terminal->tty->winsize.ws_col + c) * 2;

            uint8_t chr = characterPos[0];
            uint8_t color = characterPos[1];

            gfx_put_char_at(chr, c, r, 0, 0xFFFFFFFF);
        }
    }
}

static void fbterminal_addCharacter(terminal_t* terminal, uint8_t character)
{
    gfx_put_char_at(character, terminal->current_column, terminal->current_line, 0, 0xFFFFFFFF);
}

static void fbterminal_moveCursor(terminal_t* terminal, uint16_t old_line, uint16_t old_column, uint16_t line, uint16_t column)
{
    //restore old place
    uint8_t* character_old = terminal->buffer + (old_line * terminal->tty->winsize.ws_col + old_column) * 2;
    gfx_put_char_at(*character_old, old_column, old_line, 0, 0xFFFFFFFF);

    //put cursor to new place
    uint8_t* character_new = terminal->buffer + (line * terminal->tty->winsize.ws_col + column) * 2;
    gfx_put_char_at(*character_new, column, line, 0xFFFF0000, 0);
}