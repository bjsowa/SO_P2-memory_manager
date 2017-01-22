#include <stdio.h>

#include "stats.h"
#include "structs.h"

int areasCreated = 0;

void showAddress(void* ptr)
{
	unsigned long int add = (unsigned long int)ptr;
	unsigned long int page = add >> 12;
	unsigned long int offset = add % 4096;
	printf("page: %lu\n", page);
	printf("offset: %lu\n", offset); 
}

void printStats()
{
	printf("Areas Created: %d\n", areasCreated);
}

void printBlocks()
{
	int i = 1;
	area* currentArea = firstArea;
	while( currentArea != NULL )
	{
		printf("AREA %d\n\n", i);
		int j = 1;
		block* currentBlock = currentArea->firstBlock;
		while( currentBlock != NULL )
		{
			printf("BLOCK %d\nTYPE: ", j);
			if( currentBlock->size < 0 )
				printf("FREE\n");
			else 
				printf("TAKEN\n");

			printf("SIZE: %d\n\n", abs(currentBlock->size));
			currentBlock = currentBlock->next;
			j++;
		}
		currentArea = currentArea->next;
		i++;
	}
}