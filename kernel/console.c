#include "terminal.h"
#include "device.h"
#include "serial.h"
#include "devfs.h"
#include "alloc.h"
#include "common.h"
#include "list.h"
#include "fifobuffer.h"
#include "gfx.h"
#include "log.h"
#include "termios.h"
#include "keymap.h"
#include "console.h"

#define VT_ACTIVATE	0x5606

static terminal_t* g_terminals[TERMINAL_COUNT];
terminal_t* g_active_terminal = NULL;

static uint8_t g_key_modifier = 0;


static BOOL console_open(File *file, uint32_t flags);
static void console_close(File *file);
static int32_t console_ioctl(File *file, int32_t request, void * argp);

static uint8_t get_character_for_scancode(KeyModifier modifier, uint8_t scancode);
static void process_scancode(uint8_t scancode);

/*
 *  Initialize the console, and then register a character device that represents it.
 */
void console_initialize(BOOL graphicMode)
{
    for (int i = 0; i < TERMINAL_COUNT; ++i)
    {
        terminal_t* terminal = NULL;
        filesystem_node* ttyNode = ttydev_create();
        if (ttyNode)
        {
            TtyDev* ttyDev = (TtyDev*)ttyNode->private_node_data;
        
            terminal = terminal_create(ttyDev, graphicMode);
        }
        
        g_terminals[i] = terminal;
    }

    console_set_active_terminal_index(0);

    Device device;
    memset((uint8_t*)&device, 0, sizeof(Device));
    device.device_type = FT_CHARACTER_DEVICE;
    device.open = console_open;
    device.close = console_close;
    device.ioctl = console_ioctl;
    devfs_register_device(&device);
}


/* Send a character to the console by it's keyboard scancode... */
void console_send_key(uint8_t scancode)
{
    process_scancode(scancode);

    uint8_t character = get_character_for_scancode(g_key_modifier, scancode);

    uint8_t keyRelease = (0x80 & scancode); //ignore release event

    if (character > 0 && keyRelease == 0)
    {
        terminal_send_key(g_active_terminal, g_key_modifier, character);
    }

}

/* Not implemented yet... */
static BOOL console_open(File *file, uint32_t flags)
{
    return TRUE;
}

/* Not implemented yet... */
static void console_close(File *file)
{
    
}

static int32_t console_ioctl(File *file, int32_t request, void * argp)
{
    if (request == VT_ACTIVATE)
    {
        uint32_t index = (uint32_t)argp - 1;
        terminal_t* terminal = console_get_terminal(index);
        if (terminal)
        {
            console_set_active_terminal(terminal);
        }
        return 0;
    }
    return -1;
}

void console_set_active_terminal_index(uint32_t index)
{
    if (index >= 0 && index < TERMINAL_COUNT)
    {
        console_set_active_terminal(g_terminals[index]);
    }
}

void console_set_active_terminal(terminal_t* terminal)
{
    g_active_terminal = terminal;

    gfx_fill(0xFFFFFFFF);

    if (g_active_terminal->refresh_function)
    {
        g_active_terminal->refresh_function(g_active_terminal);
    }

    if (g_active_terminal->move_cursor_function)
    {
        g_active_terminal->move_cursor_function(g_active_terminal, g_active_terminal->current_line, g_active_terminal->current_column, g_active_terminal->current_line, g_active_terminal->current_column);
    }
}

terminal_t* console_get_terminal_by_master(filesystem_node* master_node)
{
    for (int i = 0; i < TERMINAL_COUNT; ++i)
    {
        terminal_t* terminal = g_terminals[i];
        
        if (terminal->tty->master_node == master_node)
        {
            return terminal;
        }
    }

    return NULL;
}

terminal_t* console_get_terminal_by_slave(filesystem_node* slave_node)
{
    for (int i = 0; i < TERMINAL_COUNT; ++i)
    {
        terminal_t* terminal = g_terminals[i];
        
        if (terminal->tty->slave_node == slave_node)
        {
            return terminal;
        }
    }

    return NULL;
}

terminal_t* console_get_terminal(uint32_t index)
{
    if (index >= 0 && index < TERMINAL_COUNT)
    {
        return g_terminals[index];
    }

    return NULL;
}

static uint8_t get_character_for_scancode(KeyModifier modifier, uint8_t scancode)
{
    if ((modifier & KM_LeftShift) || (modifier & KM_RightShift))
    {
        return g_key_shift_map[scancode];
    }

    return g_key_map[scancode];
}

static void process_scancode(uint8_t scancode)
{
    uint8_t lastBit = scancode & 0x80;

    scancode &= 0x7F;

    if (lastBit)
    {
        //key release

        switch (scancode)
        {
        case KEY_LEFTSHIFT:
            g_key_modifier &= ~KM_LeftShift;
            break;
        case KEY_RIGHTSHIFT:
            g_key_modifier &= ~KM_RightShift;
            break;
        case KEY_CTRL:
            g_key_modifier &= ~KM_Ctrl;
            break;
        case KEY_ALT:
            g_key_modifier &= ~KM_Alt;
            break;
        }
    }
    else
    {
        //key pressed

        switch (scancode)
        {
        case KEY_LEFTSHIFT:
            g_key_modifier |= KM_LeftShift;
            break;
        case KEY_RIGHTSHIFT:
            g_key_modifier |= KM_RightShift;
            break;
        case KEY_CTRL:
            g_key_modifier |= KM_Ctrl;
            break;
        case KEY_ALT:
            g_key_modifier |= KM_Alt;
            break;
        }

        if ((g_key_modifier & KM_Ctrl) == KM_Ctrl)
        {
            int ttyIndex = scancode - KEY_F1;
            
            console_set_active_terminal_index(ttyIndex);
        }
    }
}
