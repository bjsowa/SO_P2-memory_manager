#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include "dynmem.h"

void showAddress(void* ptr)
{
	unsigned long int add = (unsigned long int)ptr;
	unsigned long int page = add >> 12;
	unsigned long int offset = add % 4096;
	printf("page: %lu\n", page);
	printf("offset: %lu\n", offset); 
}

int main()
{
	void* x = malloc(5 * sizeof(int));
	showAddress(x);
	void* y = malloc(5 * sizeof(int));
	showAddress(y);
}