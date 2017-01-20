#include <stdio.h>
#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>
#include "dynmem.h"

typedef struct block block;

struct block {
	block* prev;
	block* next;
	size_t size;
};

const size_t blockSize = sizeof(block);

block createBlock(size_t size)
{
	block b;
	b.prev = NULL;
	b.next = NULL;
	b.size = size;
	return b;
}

void* malloc(size_t size)
{
	size_t pageSize = getpagesize();
	printf("blockSize: %d\n", (int)blockSize);
	printf("size to allocate: %d\n", (int)size);

	size += blockSize;
	size += (pageSize - (size % pageSize)) % pageSize;

	printf("size allocated: %d\n", (int)size);

	void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if( ptr == MAP_FAILED ){
		printf("mmap error\n");
		return NULL;
	}

	block b = createBlock(size - blockSize);
	block* ptr1 = (block*)ptr;
	ptr1[0] = b;

	return ptr + blockSize;
}

void* calloc(size_t count, size_t size)
{
	return malloc(count * size);
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
	ptr -= blockSize;
	block* ptr1 = (block*)ptr;
	block b = ptr1[0];

	if( munmap(ptr,b.size + blockSize) == -1 ){
		perror()
	}
}