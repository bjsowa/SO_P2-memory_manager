#ifndef DYNMEM_H
#define DYNMEM_H

void* malloc(size_t size);
void* calloc(size_t count, size_t size);
void* realloc(void *ptr, size_t size);
int posix_memalign(void **memptr, size_t alignment, size_t size);
void free1(void *ptr);

#endif