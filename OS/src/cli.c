#include "cli.h"
#include "keyboard.h"
#include "terminal.h"
#include "kheap.h"
#include "kheap.h"
#include "common.h"
#include "string.h"

uint8_t* cmdbuf;
uint8_t* argbuf;
uint8_t cmdidx = 0;
uint8_t argidx = 0;

uint8_t initialised = 0;
uint8_t is_cmd = 1;
uint8_t d_from_prompt = 0;

char* dir;

void clear_cmd();
void clear_arg();

void handle_command()
{
    printk("\n");
    if (!strcmp(cmdbuf, "print_heap"))
    {
        clear_cmd();
        clear_arg();
    }
    else
    {
        printk("Unknown command <%s>. Please try again\n", cmdbuf);
    }
    
    clear_cmd();
    clear_arg();
    printk("%s", dir);
    d_from_prompt = 0;
}

void clear_cmd()
{
    memset(cmdbuf, 0x0, 256);
    cmdidx = 0;
    is_cmd = 1;
}

void clear_arg()
{
    memset(argbuf, 0x0, 256);
    argidx = 0;
    is_cmd = 1;
}

void initialise_cli()
{
    cmdbuf = (uint8_t*)kmalloc(256);
    argbuf = (uint8_t*)kmalloc(256);
    clear_cmd();
    clear_arg();
    terminal_clear();
    dir = "/$ ";
    printk("%s", dir);
    initialised = 1;
}

void cli_send_packet(keyboard_packet_t* packet)
{
    if (!initialised) return;
    if (packet->keycode == '\n') handle_command();
    else if (packet->keycode == '\t') return;
    else if (packet->keycode == '\b')
    {
        if (!d_from_prompt) return;
        printk("%c", '\b');
        d_from_prompt--;
        if (is_cmd)
        {
            cmdidx--;
            cmdbuf[cmdidx] = 0;
        }
        else
        {
            argidx--;
            argbuf[argidx] = 0;
        }
        
    }
    else if (packet->keycode == ' ')
    {
        d_from_prompt++;
        if (is_cmd)
        {
            printk(" ");
            cmdbuf[cmdidx] = '\0';
            is_cmd = 0;
        }
        else
        {
            printk(" ");
            argbuf[argidx] = ' ';
            argidx++;
        }
        
    }
    else if (packet->keycode == 0) return;
    else
    {
        d_from_prompt++;
        printk("%c", packet->keycode);
        if (is_cmd)
        {
            cmdbuf[cmdidx] = packet->keycode;
            cmdidx++;
        }
        else
        {
            argbuf[argidx] = packet->keycode;
            argidx++;
        }
    }
}