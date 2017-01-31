#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/mman.h>
#include <unistd.h>

#include "structs.h"
#include "stats.h"

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
	a.size = size;
	return a;
}

void divideBlock(void* ptr, size_t size)
{
    block* fullBlock = (block*) ptr;
    fullBlock->size = abs(fullBlock->size);

    if( fullBlock->size == size ) {
    	takenSpace += fullBlock->size; //STAT
        if(takenSpace>maxTakenSpace)
            maxTakenSpace=takenSpace;
       	freeSpace -= fullBlock->size; //STAT
        if(freeSpace>maxFreeSpace)
            maxFreeSpace=freeSpace;
    	return;
    }

    block* freeBlock = (block*) (ptr + blockSize + size);

    if( fullBlock->size - size < blockSize + 1 ){
        if( fullBlock->next != NULL && fullBlock->next->size < 0 ){
        	*freeBlock = initializeBlock( abs(fullBlock->next->size) + fullBlock->size - size, true );
        	fullBlock->next = fullBlock->next->next;
        	freeSpace -= size; //STAT
            if(freeSpace>maxFreeSpace)
                maxFreeSpace=freeSpace;

            ++blocksDivided; //STAT
        }
        else {
        	takenSpace += fullBlock->size; //STAT
            if(takenSpace>maxTakenSpace)
                maxTakenSpace=takenSpace;
        	freeSpace -= fullBlock->size; //STAT
            if(freeSpace>maxFreeSpace)
                maxFreeSpace=freeSpace;

        	return;
        }
    }
    else {
    	*freeBlock = initializeBlock( fullBlock->size - size - blockSize, true );
    	freeSpace -= size + blockSize; //STAT
        if(freeSpace>maxFreeSpace)
            maxFreeSpace=freeSpace;

        ++blocksDivided; //STAT
    }

    takenSpace += size; //STAT
    if(takenSpace>maxTakenSpace)
        maxTakenSpace=takenSpace;

    freeBlock->prev = fullBlock;
    freeBlock->next = fullBlock->next;

    if(fullBlock->next != NULL)
        fullBlock->next->prev = freeBlock;
    fullBlock->next = freeBlock;
    fullBlock->size = size;

    mergeFreeBlocks(freeBlock);
}

void* mergeFreeBlocks(block* ptr)
{
	if( ptr->prev != NULL && ptr->prev->size < 0 ){
		ptr->prev->next = ptr->next;
		ptr->prev->size += ptr->size - (ssize_t)blockSize;
		ptr = ptr->prev;
		if( ptr->next != NULL ) ptr->next->prev = ptr;

		freeSpace += blockSize; //STAT
        if(freeSpace>maxFreeSpace)
            maxFreeSpace=freeSpace;

        ++blocksMerged; //STAT
	}
	if( ptr->next != NULL && ptr->next->size < 0 ){
		ptr->size += ptr->next->size - (ssize_t)blockSize;
		ptr->next = ptr->next->next;
		if( ptr->next != NULL ) ptr->next->prev = ptr;

		freeSpace += blockSize; //STAT
        if(freeSpace>maxFreeSpace)
            maxFreeSpace=freeSpace;

        ++blocksMerged; //STAT
	}
	return ptr;
}

void createArea(void* ptr, size_t asize, size_t bsize)
{
	area a = initializeArea(asize);
	
	area* newArea = (area*) ptr;
	*newArea = a;

	block* fullBlock = (block*) (newArea + 1);
	size_t freeBlockSize = asize - areaSize - blockSize - bsize;
	if( freeBlockSize > blockSize ){
		*fullBlock = initializeBlock(bsize,false);

		block* freeBlock = (block*) (ptr + areaSize + blockSize + bsize);
		*freeBlock = initializeBlock(freeBlockSize - blockSize,true);

		fullBlock->next = freeBlock;
		freeBlock->prev = fullBlock;

		freeSpace += freeBlockSize - blockSize; //STAT
        if(freeSpace>maxFreeSpace)
            maxFreeSpace=freeSpace;
	}
	else 
		*fullBlock = initializeBlock(asize - areaSize - blockSize,false);

	newArea->firstBlock = fullBlock;

	if( lastArea == NULL ){
		firstArea = newArea;
		lastArea = newArea;
	}
	else {
		lastArea->next = newArea;
		newArea->prev = lastArea;
		lastArea = newArea;
	}

	takenSpace += fullBlock->size; //STAT
    if(takenSpace>maxTakenSpace)
        maxTakenSpace=takenSpace;

	++areasCreated; //STAT
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
