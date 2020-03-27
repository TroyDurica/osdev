#ifndef STRING_H
#define STRING_H 0

#include "stddef.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern int memcmp(const void *, const void *, size_t);
extern void *memchr(const void *, int, size_t);
extern void *memmove(void *, const void *, size_t);
extern void *memcpy(void *, const void *, size_t);
extern void *memset(void*, int, size_t);
extern size_t strlen(const char*);
extern void itoa(char*, sint32_t, uint32_t);
extern char *strcat(char *, const char *);
extern char *strcpy(char *, const char *);
extern int strcmp(const char *, const char *);
extern int atoi (const char *);

#ifdef __cplusplus
}
#endif

#endif