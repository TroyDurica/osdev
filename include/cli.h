#ifndef CLI_H
#define CLI_H 0

#include "stdint.h"
#include "keyboard.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern void initialise_cli();
extern void cli_send_packet(keyboard_packet_t*);

#ifdef __cplusplus
}
#endif

#endif