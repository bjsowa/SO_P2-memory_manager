#include <stddef.h>
#include "dynmem.h"

void* malloc(size_t size)
{
	return NULL;
}

void* calloc(size_t count, size_t size)
{
	return NULL;
}

void* realloc(void* ptr, size_t size)
{
	return NULL;
}

int posix_memalign(void** memptr, size_t alignment, size_t size)
{
	return 0;
}

void free(void* ptr)
{

}