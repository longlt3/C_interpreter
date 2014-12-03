#ifndef DEFINITION_H
#define DEFINITION_H

#include "stddef.h"

void *tci_memcpy ( void * destination, const void * source, size_t num );
void *tci_memset(void *str, int c, size_t n);
void *tci_malloc (size_t size);
void tci_free (void* ptr);

#endif
