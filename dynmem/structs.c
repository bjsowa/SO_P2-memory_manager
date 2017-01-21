#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>
#include "structs.h"

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

}


void createArea(void* ptr, size_t asize, size_t bsize)
{
	area a = initializeArea(asize);
	
	area* ptr1 = (area*) ptr;
	*ptr1 = a;

	block* ptr2 = (block*) (ptr1 + 1);
	size_t freeBlockSize = asize - areaSize - blockSize - bsize;
	if( freeBlockSize > blockSize ){
		block b = initializeBlock(bsize,false);
		*ptr2 = b;

		b = initializeBlock(freeBlockSize,true);
		block* ptr3 = (block*) (ptr + areaSize + blockSize + bsize);
		*ptr3 = b;

		ptr2->next = ptr3;
		ptr3->prev = ptr2;
	}
	else {
		block b = initializeBlock(asize - areaSize,false);
		*ptr2 = b;
	}

	ptr1->firstBlock = ptr2;

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
