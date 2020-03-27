#ifndef INITRD_H
#define INITRD_H 0

#include "vfs.h"
#include "stdint.h"

typedef struct
{
   	uint32_t nfiles;
} initrd_header_t;

typedef struct
{
	uint8_t magic;
	sint8_t name[64];
	uint32_t offset;
	uint32_t length;
} initrd_file_header_t;

#ifdef __cplusplus
extern "C"
{
#endif

extern fs_node_t *initialise_initrd(uint32_t location, uint32_t end);

#ifdef __cplusplus
}
#endif

#endif