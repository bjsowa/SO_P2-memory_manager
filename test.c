#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "dynmem.h"
#include "stats.h"

typedef struct pair pair;

struct pair {
    double a;
    double b;
};

int main()
{
    printf("1) Tablica z alfabetem:\n");
    char* alfabet = malloc(51);
    strcpy(alfabet, "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z");
    printf("%s\n\n", alfabet);

    printf("2) Tablica intow\n");
    int* x = malloc(5 * sizeof(int));
    int five=5;

    printf("x=[ ");
    while(five--){
        *(x+five)=five;
        printf("%d ", *(x+five));
    }
    printf("]\n\n");

    printf("3) Tablica struktur \"pair\":\n");
    pair* p = malloc(2 * sizeof(pair));
    p->a=0.0;
    p->b=1.0;
    (p+1)->a=1.5;
    (p+1)->b=2.5;
    printf("[ (%lf, %lf) (%lf, %lf) ]\n\n",p->a,p->b,(p+1)->a,(p+1)->b);

    printf("\nMalloc:\n\n");

    printBlocks();
    printStats();

    printf("\nRealloc:\n\n");

    //int* i = (int*) x;
    //i[300] = 5213;
    x = realloc(x,4000);
    //i = (int*) x;
    //printf("%d\n", i[300]);

    p = realloc(p,1000);
    int* y = malloc(250 * sizeof(double));

    printBlocks();
    printStats();

    int* z = malloc(2*sizeof(char));

    printBlocks();
    printStats();

    printf("\nFree:\n\n");

    free(z);
    free(alfabet);

    printBlocks();
    printStats();

    free(y);
    free(x);

    printBlocks();
    printStats();

    free(p);

    printBlocks();
    printStats();
}
