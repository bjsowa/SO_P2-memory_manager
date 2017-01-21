#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>
#include "structs.h"

//wskaźniki na pierwszy i ostatni obszar 
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

void divideBlock(void* ptr, size_t size)
{

}


void createArea(void* ptr, size_t asize, size_t bsize)
{
	area a;
	a.prev = lastArea;
	a.next = NULL;
	a.size = asize;
	
	area* ptr1 = (area*) ptr;
	*ptr1 = a;

	block* ptr2 = (block*) (ptr1 + 1);
	block b = createBlock(bsize);
	b.free = false;
	*ptr2 = b;
	ptr1->firstBlock = ptr2;

	size_t freeBlockSize = asize - areaSize - blockSize - bsize;
	if( freeBlockSize > blockSize ){
		block b1 = createBlock(freeBlockSize);
		b1.free = true;
		block* ptr3 = (block*) (ptr + areaSize + blockSize + bsize);
		*ptr3 = b1;

		ptr2->next = ptr3;
		ptr3->prev = ptr2;
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
	area* currentArea = firstArea;
	while( currentArea != NULL )
	{
		block* currentBlock = currentArea->firstBlock;
		while( currentBlock != NULL )
		{
			if( currentBlock->free && currentBlock->size >= size )
				return currentBlock;
			currentBlock = currentBlock->next;
		}
		currentArea = currentArea->next;
	}
	return NULL;
}
