#include "terminal.h"
#include "vgaterminal.h"
#include "common.h"
#include "ttydev.h"

terminal_t *terminal_create(ttydev_t *ttydev) {
    terminal_t *terminal = kmalloc(sizeof(terminal_t));
    memset((uint8_t *)terminal, 0, sizeof(terminal_t));

    terminal->tty = ttydev;

    terminal->terminal_send = vga_putc;
    terminal->terminal_get = NULL;
}

void terminal_destroy(terminal_t *terminal) {
    kfree(terminal);
}