#pragma once

#include "ttydev.h"
#include "bool.h"

typedef struct _terminal_t terminal_t;

typedef void (*terminal_send_t)(terminal_t *terminal, char character);
typedef void (*terminal_get_t)(terminal_t *terminal, char *character);

typedef struct _terminal_t {
    ttydev_t *tty;
    bool is_disabled;
    terminal_send_t terminal_send;
    terminal_get_t terminal_get;
} terminal_t;