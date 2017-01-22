#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include "dynmem.h"
#include "stats.h"

int main()
{
	void* x = malloc(5 * sizeof(int));
	showAddress(x);
	void* y = malloc(1000 * sizeof(int));
	showAddress(y);
	void* z = malloc(5 * sizeof(int));
	showAddress(z);

	printStats();
}