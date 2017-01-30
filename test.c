#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "dynmem.h"
#include "stats.h"

int main()
{
    srand(time(NULL));

    printf("1) Tablica z alfabetem:\n");
    char* alfabet = malloc(51);
    strcpy(alfabet, "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z");
    printf("%s\n", alfabet);

    printf("2) Tablica intow\n");
    int* x = malloc(5 * sizeof(int));
    int five=5;

    printf("x=[ ");
    while(five--){
        *(x+five)=five;
        printf("%d ", *(x+five));
    }
    printf("]\n");

    printf("Malloc:\n");

	printBlocks();

	int* i = (int*) x;
	i[300] = 5213;
	x = realloc(x,500);
	i = (int*) x;
	printf("%d\n", i[300]);

	printStats();

    printf("Realloc:\n");

	printBlocks();
}
