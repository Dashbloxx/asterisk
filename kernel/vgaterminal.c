#include "stdint.h"
#include "terminal.h"

uint16_t *video_memory = (uint16_t *)0xB8000;

uint8_t cursor_x = 0, cursor_y = 0;

static void vga_scroll() {
    uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);

    if(cursor_y >= 25) {
        int i;
        for(i = 0 * 80; i < 24 * 80; i++) {
            video_memory[i] = video_memory[i + 80];
        }

        for(i = 24 * 80; i < 25 * 80; i++) {
            video_memory[i] = blank;
        }

        cursor_y = 24;
    }
}

void vga_putc(terminal_t *terminal, char character) {
    uint8_t background_color = 0;
    uint8_t foreground_color = 15;

    uint8_t attribute_byte = (background_color << 4) | (foreground_color & 0x0F);

    uint16_t attribute = attribute_byte << 8;
    uint16_t *location;

    if(character == 0x08 && cursor_x) {
        cursor_x--;
    } else if(character == 0x09) {
        cursor_x = (cursor_x + 8) & ~(8 - 1);
    } else if(character == '\r') {
        cursor_x = 0;
    } else if(character == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if(character >= ' ') {
        location = video_memory + (cursor_y * 80 + cursor_x);
        *location = character | attribute;
        cursor_x++;
    }

    if(cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }

    vga_scroll();

    uint16_t cursor_location = cursor_y * 80 + cursor_x;
    outb(0x3D4, 14);
    outb(0x3D5, cursor_location >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cursor_location);
}