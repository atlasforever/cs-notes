#include <stddef.h>

void *memset(void *dest, int value, size_t count)
{
    unsigned char *p = dest;

    while (count-- > 0) {
        *p++ = (unsigned char)value;
    }

    return dest;
}

void *memcpy(void *dest, const void *src, size_t count)
{
    unsigned char *d = dest;
    const unsigned char *s = src;

    while (count-- > 0) {
        *d++ = *s++;
    }

    return dest;
}