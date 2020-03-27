#ifndef KEYBOARD_H
#define KEYBOARD_H 0

#include "stdint.h"

typedef struct
{
    uint8_t shift    : 1;
    uint8_t alt      : 1;
    uint8_t ctrl     : 1;
    uint8_t fn       : 1;
    uint8_t pressed  : 1;
    uint8_t released : 1;
    uint8_t unused   : 2;
    uint8_t keycode  : 8;
} keyboard_packet_t;


#ifdef __cplusplus
extern "C"
{
#endif

extern void initialise_keyboard();

#ifdef __cplusplus
}
#endif

#endif