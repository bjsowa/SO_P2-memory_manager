#include <stddef.h>
#include <stdio.h>
#include "dynmem.h"

int main()
{
	int* a = malloc(3);
	calloc(3,2);

	printf("%d\n", a);
}