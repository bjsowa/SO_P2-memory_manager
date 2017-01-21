#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>
#include "dynmem.h"

typedef struct block block;
typedef struct area area;

struct block {
	block* prev;
	block* next;
	size_t size;
	bool free;
};

struct area {
	area* prev;
	area* next;
	block* firstBlock;
	size_t size;
};

const size_t blockSize = sizeof(block);
const size_t areaSize = sizeof(area);

area* firstArea = NULL;
area* lastArea = NULL;

block createBlock(size_t size)
{
	block b;
	b.prev = NULL;
	b.next = NULL;
	b.size = size;
	return b;
}


void createArea(void* ptr, size_t asize, size_t bsize)
{
	area a;
	a.prev = lastArea;
	a.next = NULL;
	a.size = asize;
	
	area* ptr1 = (area*) ptr;
	ptr1[0] = a;

	block* ptr2 = (block*) (ptr1 + 1);
	block b = createBlock(bsize);
	b.free = false;
	ptr2[0] = b;

	size_t freeBlockSize = asize - areaSize - blockSize - bsize;
	if( freeBlockSize > blockSize ){
		block b1 = createBlock(freeBlockSize);
		b1.free = true;
		block* ptr3 = (block*) (ptr + areaSize + blockSize + bsize);
		ptr3[0] = b1;

		ptr2[0].next = ptr3;
		ptr3[0].prev = ptr2;
	}

	if( lastArea == NULL ){
		firstArea = ptr1;
		lastArea = ptr1;
	}
	else {
		lastArea->next = ptr1;
		ptr1->prev = lastArea;
		lastArea = ptr1;
	}
}


//searches for a free block of at least given size
block* sfree(size_t size)
{
	return NULL;
}

void* malloc(size_t size)
{
	size_t pageSize = (size_t)getpagesize();
	printf("blockSize: %d\n", (int)blockSize);
	printf("areaSize: %d\n", (int)areaSize);
	printf("size to allocate: %d\n", (int)size);

	block* dest = sfree(size);

	if( dest == NULL ){
		size_t size1 = size;
		size1 += blockSize + areaSize;
		size1 += (pageSize - (size1 % pageSize)) % pageSize;

		void* ptr = mmap(NULL, size1, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if( ptr == MAP_FAILED ){
			perror("mmap error\n");
			return NULL;
		}

		printf("size allocated: %d\n", (int)size1);

		createArea(ptr, size1, size);

		return ptr + areaSize + blockSize;
	}	
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


}