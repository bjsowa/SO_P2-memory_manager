#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include "dynmem.h"

int main()
{
	int* x = malloc(4073);
	x[0] = 15;
	printf("%d\n", x[0]);
}