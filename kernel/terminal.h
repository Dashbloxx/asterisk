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