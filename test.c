#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "dynmem.h"
#include "stats.h"
#include "structs.h"

#define ITERATIONS 10
#define THREADS 1

int makeRandom(int a, int b)
{
    return rand()%(b-a+1) + a;
}

void* test(void* data)
{
    printBlocks();
    //printStats();

    int num = *(int*)data;

    for(int iter = 0; iter < ITERATIONS; iter++)
    {
        int testCount = makeRandom(1,10);


        pthread_mutex_lock(&printingMutex); //MUTEX
        printf("Wątek %d tworzy tablicę wskaźników\n", num);
        pthread_mutex_unlock(&printingMutex); //MUTEX

        char** ptrs = calloc(testCount, sizeof(char*));

        size_t bytes[testCount];

        for(int i=0;i<testCount;i++)
        {
            bytes[i] = makeRandom((1<<7),(1<<18));

            pthread_mutex_lock(&printingMutex); //MUTEX
            printf("Wątek %d tworzy %lu bajtów\n", num, bytes[i]);
            pthread_mutex_unlock(&printingMutex); //MUTEX

            char* pointer = realloc(NULL, bytes[i]);
/*            bytes[i] = makeRandom((1<<4), bytes[i]/2);
            printf("Wątek %d reallocuje %lu bajtów\n", num, bytes[i]);
            pointer = realloc(pointer, bytes[i]);*/

            ptrs[i] = pointer;
            for(unsigned int j = 0; j < bytes[i]; j++)
            {
                pointer[j] = j%('z'-'a'+1) + 'a';
            }
        }

        //printBlocks();
        //printStats();

        for(int i=0;i<testCount; i++)
        {
            if(ptrs[i] == NULL)
            {
                printf("WKAŹNIK DO FREE JEST NULL!\n");
            }

            pthread_mutex_lock(&printingMutex); //MUTEX
            printf("Wątek %d zwalnia pole %lu bajtowe\n", num, bytes[i]);
            pthread_mutex_unlock(&printingMutex); //MUTEX

            free(ptrs[i]);
        }

        //printBlocks();`

        pthread_mutex_lock(&printingMutex); //MUTEX
        printf("Wątek %d zwalnia tablicę wskaźników\n", num);
        pthread_mutex_unlock(&printingMutex); //MUTEX

        free(ptrs);

        printBlocks();
    }

    pthread_mutex_lock(&printingMutex); //MUTEX
    printf("Wątek %d kończy działanie\n", num);
    pthread_mutex_unlock(&printingMutex); //MUTEX

    return NULL;
}

int main()
{
    srand(time(NULL));

    printf("Mallocuje wskaźnik\n");
    char* ptr = malloc(16);
    strcpy(ptr,"abcdefghijklmnop");
    printf("ptr = %p\n", ptr);

    printf("Callocuję threadInfo\n");
    pthread_t* threadInfo = calloc(THREADS, sizeof(pthread_t));

    int ids[THREADS];
    for(int i = 0;i<THREADS;i++)
    {
        ids[i]=i;

        printBlocks();

        printf("Tworzę wątek %d\n", i);
        pthread_create(&threadInfo[i], NULL, test, (void*)&ids[i]);


    }
    for(int i=0;i<THREADS;i++)
    {
        void* retVal;
        printf("Wątek main czeka na wątek %d\n", i);
        pthread_join(threadInfo[i], &retVal);
    }

    printf("Zwalniam threadInfo\n");
    free(threadInfo);
    printf("Zwalniam mainowy ptr\n");
    free(ptr);

    printBlocks();
    printStats();

    return 0;
}
