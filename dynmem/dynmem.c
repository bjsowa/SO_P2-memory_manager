#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#include "dynmem.h"
#include "structs.h"
#include "stats.h"

void* malloc(size_t size)
{
	if( size == 0 ) return NULL;

	size += (alignment - (size % alignment)) % alignment;

	size_t pageSize = (size_t)getpagesize();
	block* dest = sfree(size);

	if( dest == NULL ){
		size_t size1 = size;
		size1 += blockSize + areaSize;
		size1 += (pageSize - (size1 % pageSize)) % pageSize;

		void* ptr = mmap(NULL, size1, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if( ptr == MAP_FAILED ){
			perror("mmap error");
			return NULL;
		}

		createArea(ptr, size1, size);

		return ptr + areaSize + blockSize;
	}	
	else {
		divideBlock(dest, size);

		void* ptr = (void*) dest;
		return ptr + blockSize;
	}
}

void* calloc(size_t count, size_t size)
{
	return malloc(count * size);
}

void* realloc(void* ptr, size_t size)
{
	if( size == 0 ) return NULL;
	size += (alignment - (size % alignment)) % alignment;

	block* blockPlace = (block*)(ptr - blockSize);

	if( size < blockPlace->size ){
		divideBlock(blockPlace,size);
		return ptr;
	}
	else if( size > blockPlace->size ){
		if( blockPlace->next != NULL && blockPlace->next->size < 0 
			&& blockPlace->size - blockPlace->next->size + blockSize >= size ){

			blockPlace->size += blockSize - blockPlace->next->size;
			blockPlace->next = blockPlace->next->next;
			divideBlock(blockPlace,size);

			return blockPlace;
		}

		size_t oldSize = abs(blockPlace->size);
		free(ptr);
		void* newBlock = malloc(size);
		memmove(newBlock, ptr, oldSize);

		return newBlock;
	}

	return ptr;
}

int posix_memalign(void** memptr, size_t alignment, size_t size)
{
	return 0;
}

void free(void* ptr)
{
	ptr -= blockSize;
	block* freeBlock = (block*)ptr;
	
	freeBlock->size *= -1;
	freeBlock = mergeFreeBlocks(freeBlock);

	if( freeBlock->prev == NULL && freeBlock->next == NULL && freeSpace - freeBlock->size >= UNMAP_AREA_COND ){
		uintptr_t pageSize = (uintptr_t)getpagesize();
		uintptr_t ptr1 = (uintptr_t)freeBlock;
		area* freeArea = (area*)(ptr1 - (ptr1 % pageSize));
		
		if( freeArea == firstArea ){
			firstArea = freeArea->next;
			firstArea->prev = NULL;
		}
		else if( freeArea == lastArea ){
			lastArea = freeArea->prev;
			lastArea->next = NULL;
		}

		if( munmap(freeArea, freeArea->size) == -1) {
			perror("munmap error");
		}
	}
}