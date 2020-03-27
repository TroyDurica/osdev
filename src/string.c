#include "string.h"

#include "stddef.h"
#include "stdint.h"

char *strcat(char *dest, const char *src)
{
    char *rdest = dest;

    while (*dest) dest++;
    while (*dest++ = *src++);
    return rdest;
}

void itoa(char *buf, sint32_t n, uint32_t base)
{
    unsigned long int tmp;
    int i, j;

    tmp = n;
    i = 0;

    do {
        tmp = n % base;
        buf[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
    } while (n /= base);
    buf[i--] = 0;

    for (j = 0; j < i; j++, i--) {
        tmp = buf[j];
        buf[j] = buf[i];
        buf[i] = tmp;
    }
} 


void *memcpy(void *dst, const void *src, size_t n)
{
    uint8_t *p = (uint8_t *)src;
    uint8_t *q = (uint8_t *)dst;
    uint8_t *end = p + n;

    while (p != end)
    {
        *q++ = *p++;
    }

    return dst;
}

void *memmove(void *dst, const void *src, size_t n)
{
    uint8_t *p = (uint8_t *)src;
    uint8_t *q = (uint8_t *)dst;
    uint8_t *end = p + n;

    if (q > p && q < end)
    {
        p = end;
        q += n;

        while (p != src)
        {
            *--q = *--p;
        }
    }
    else
    {
        while (p != end)
        {
            *q++ = *p++;
        }
    }

    return dst;
}

void *memchr(const void *buf, int c, size_t n)
{
    uint8_t *p = (uint8_t *)buf;
    uint8_t *end = p + n;

    while (p != end)
    {
        if (*p == c)
        {
            return p;
        }

        ++p;
    }

    return 0;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    const uint8_t *byte1 = (const uint8_t *)s1;
    const uint8_t *byte2 = (const uint8_t *)s2;
    while ((*byte1 == *byte2) && (n > 0))
    {
        ++byte1;
        ++byte2;
        --n;
    }

    if (n == 0)
    {
        return 0;
    }
    return *byte1 - *byte2;
}

void *memset(void* s, int c, size_t n)
{
	uint8_t *p = (uint8_t*) s;
	uint8_t *end = p + n;

	while (p != end)
	{
		*p++ = c;
	}

	return s;
}

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

char *strcpy(char *dst, const char *src)
{
    char c;
    char *p = dst;

    while ((c = *src++))
    {
        *p++ = c;
    }

    *p = '\0';
    return dst;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 == *s2)
    {
        if (*s1 == '\0')
        {
            return 0;
        }

        ++s1;
        ++s2;
    }

    return *s1 - *s2;
}

uint8_t isdigit(char c)
{
    return c >= '0' && c <= '9';
}

int atoi (const char * str)
{
    int value = 0;
    while(isdigit(*str))
    {
        value *= 10;
        value += (*str)-'0';
        str++;
    }

    return value;
}