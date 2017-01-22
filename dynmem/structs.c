#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>
#include "structs.h"
#include "stats.h"

//wskaźniki na pierwszy i ostatni obszar 
area* firstArea = NULL;
area* lastArea = NULL;

block initializeBlock(size_t size, bool free)
{
	block b;
	b.prev = NULL;
	b.next = NULL;
	b.size = free ? -1*(ssize_t)size : size;
	return b;
}

area initializeArea(size_t size)
{
	area a;
	a.prev = lastArea;
	a.next = NULL;
	return a;
}

void divideBlock(void* ptr, size_t size)
{
    block* fullBlock = (block*) ptr;
    fullBlock->size *= -1;
    if( fullBlock->size - (ssize_t) size - (ssize_t) blockSize < 1 )
        return;

    block freeBlock = initializeBlock(fullBlock->size - size - blockSize, true);
    block* freeSpace = (block*) (ptr + blockSize + size);
    *freeSpace = freeBlock;

    freeSpace->prev = fullBlock;
    freeSpace->next = fullBlock->next;

    if(fullBlock->next != NULL)
        fullBlock->next->prev = freeSpace;
    fullBlock->next = freeSpace;
    fullBlock->size = size;
}

void createArea(void* ptr, size_t asize, size_t bsize)
{
	area a = initializeArea(asize);
	
	area* areaPlace = (area*) ptr;
	*areaPlace = a;

	block* blockPlace = (block*) (areaPlace + 1);
	size_t freeBlockSize = asize - areaSize - blockSize - bsize;
	if( freeBlockSize > blockSize ){
		block b = initializeBlock(bsize,false);
		*blockPlace = b;

		b = initializeBlock(freeBlockSize - blockSize,true);
		block* freeBlockPlace = (block*) (ptr + areaSize + blockSize + bsize);
		*freeBlockPlace = b;

		blockPlace->next = freeBlockPlace;
		freeBlockPlace->prev = blockPlace;
	}
	else {
		block b = initializeBlock(asize - areaSize,false);
		*blockPlace = b;
	}

	areaPlace->firstBlock = blockPlace;

	if( lastArea == NULL ){
		firstArea = areaPlace;
		lastArea = areaPlace;
	}
	else {
		lastArea->next = areaPlace;
		areaPlace->prev = lastArea;
		lastArea = areaPlace;
	}

	++areasCreated;
}

block* sfree(size_t size)
{
	area* currentArea = firstArea;
	while( currentArea != NULL )
	{
		block* currentBlock = currentArea->firstBlock;
		while( currentBlock != NULL )
		{
			if( currentBlock->size <= -1 * (ssize_t)size )
				return currentBlock;
			currentBlock = currentBlock->next;
		}
		currentArea = currentArea->next;
	}
	return NULL;
}
