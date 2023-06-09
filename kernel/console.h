#pragma once

#include "common.h"
#include "terminal.h"

#define TERMINAL_COUNT 10

typedef struct filesystem_node filesystem_node;

extern terminal_t* g_active_terminal;

void console_initialize();

void console_send_key(uint8_t scancode);

void console_set_active_terminal_index(uint32_t index);

void console_set_active_terminal(terminal_t* terminal);

terminal_t* console_get_terminal(uint32_t index);

terminal_t* console_get_terminal_by_master(filesystem_node* master_node);

terminal_t* console_get_terminal_by_slave(filesystem_node* slave_node);