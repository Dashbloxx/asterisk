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