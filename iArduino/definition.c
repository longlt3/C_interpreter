#include "definition.h"

void *tci_memcpy ( void * destination, const void * source, size_t num )
{
    return memcpy(destination, source, num);
}

void *tci_memset(void *str, int c, size_t n)
{
    return memset(str, c, n);
}

void *tci_malloc (size_t size)
{
    return malloc(size);
}

void tci_free (void* ptr)
{
    free(ptr);
}

