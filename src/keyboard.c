#include "keyboard.h"
#include "isr.h"
#include "serial.h"
#include "cli.h"

uint8_t data;

keyboard_packet_t packet;

uint8_t kblayout[128] = { 0,
    27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', 
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',  1,        
    '\\', 'z', 'x', 'c', 'v', 'b', 'n','m', ',', '.', '/',   0,                  
    '*',  0,  ' ',  0, 0,  0,
    0,   0,   0,   0,   0,   0,   0,  0,  0,  
    0,  0,  0,  0,  '-',  0,  0,  0,  '+',  0,  
    0,  0,  0,  0,  0,   0,   0,  0,  0,  0 };

uint8_t kblayout_shift[128] = { 0,
    27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', 
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',     
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\'', '`',  1,        
    '|', 'Z', 'X', 'C', 'V', 'B', 'N','M', '<', '>', '?',   0,                  
    '*',  0,  ' ',  0, 0,  0,
    0,   0,   0,   0,   0,   0,   0,  0,  0,  
    0,  0,  0,  0,  '-',  0,  0,  0,  '+',  0,  
    0,  0,  0,  0,  0,   0,   0,  0,  0,  0 };

isr_t keyboard_callback(registers_t *r)
{
    data = inb(0x60);
    if (data == 0x2A || data == 0x36)
    {
        packet.shift = 1;
        packet.pressed = 1;
        return;
    }
    else if (data == 0xAA || data == 0xB6)
    {
        packet.shift = 0;
        packet.released = 1;
        return;
    }
    else if (data & 0x80) return;

    if (packet.shift) packet.keycode = kblayout_shift[data];
    else packet.keycode = kblayout[data];
    cli_send_packet(&packet);
}

void initialise_keyboard()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
}