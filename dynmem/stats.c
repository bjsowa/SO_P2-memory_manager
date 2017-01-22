#include <stdio.h>
#include "stats.h"

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