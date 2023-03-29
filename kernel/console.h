#pragma once

#include "common.h"

#define TERMINAL_COUNT 10

typedef struct Terminal Terminal;

typedef struct filesystem_node filesystem_node;

extern Terminal* g_active_terminal;

void console_initialize(BOOL graphicMode);

void console_send_key(uint8_t scancode);

void console_set_active_terminal_index(uint32_t index);

void console_set_active_terminal(Terminal* terminal);

Terminal* console_get_terminal(uint32_t index);

Terminal* console_get_terminal_by_master(filesystem_node* master_node);

Terminal* console_get_terminal_by_slave(filesystem_node* slave_node);