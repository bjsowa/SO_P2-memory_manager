#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <pthread.h>

#include "stats.h"
#include "structs.h"

#define AREA_WIDTH 100

#define RED "\x1B[7;31m"
#define GRN   "\x1B[7;32m"
#define BLU   "\x1B[7;34m"
#define BLK "\x1B[30;40m"
#define YEL "\x1B[30;48;2;252;127;0m"
#define RESET "\x1B[0m"

int areasCreated = 0;
uint64_t takenSpace = 0;
uint64_t freeSpace = 0;
int blocksDivided = 0;
int blocksMerged = 0;

pthread_mutex_t printingMutex = PTHREAD_MUTEX_INITIALIZER;

void showAddress(void* ptr)
{
	uintptr_t add = (uintptr_t)ptr;
	uintptr_t page = add >> 12;
	uintptr_t offset = add % 4096;

    pthread_mutex_lock(&printingMutex); //MUTEX

	printf("page: %" PRIuPTR "\n", page);
	printf("offset: %" PRIuPTR "\n", offset); 

    pthread_mutex_unlock(&printingMutex); //MUTEX  
}

void printStats()
{
    pthread_mutex_lock(&printingMutex); //MUTEX

	printf("Areas Created: %d\n", areasCreated);
    printf("Taken Space: %" PRIu64 "\n", takenSpace);
    printf("Free Space: %" PRIu64 "\n", freeSpace);
	printf("Blocks Merged: %d\n", blocksMerged);
	printf("Blocks Divided: %d\n", blocksDivided); 

    pthread_mutex_unlock(&printingMutex); //MUTEX     
}

int numOfDigits(uint64_t n)
{
    int cnt = 0;
    while( n > 0 ){
        n /= 10;
        ++cnt;
    }
    return cnt;
}

void printArea(int blocks, uint64_t* blockSizes, bool* free, int* baseLength)
{
    int remainingSpace = AREA_WIDTH - blocks + 1;
    int i;
    uint64_t sum = 0;
    for(i = 0; i < blocks; i++){
        sum += blockSizes[i];
        remainingSpace -= baseLength[i];
    }

    int spaces[blocks];
    double spaceWidth = (double)sum / (double)AREA_WIDTH;

    double sizeSum = 0;
    int spacesSum = 0;
    for(i = 0; i < blocks; i++)
    {
        sizeSum += (double)blockSizes[i];
        spacesSum += baseLength[i];
        if( i != blocks-1 ) spacesSum++;

        double minSpace = (double)spacesSum * spaceWidth;

        if( sizeSum >= minSpace + spaceWidth ){
            spaces[i] = (sizeSum - minSpace - spaceWidth) / spaceWidth;
            remainingSpace -= spaces[i];
            spacesSum += spaces[i];
        }
        else spaces[i] = 0;
    }
    if( remainingSpace < 0 ){
        for(i = blocks-1; i >= 0 && remainingSpace < 0; i--)
        {
            if( -remainingSpace >= spaces[i] ){
                remainingSpace += spaces[i];
                spaces[i] = 0;
            }
            else{
                spaces[i] += remainingSpace;
                remainingSpace = 0;
            }
        }
    }
    spaces[blocks-1] += remainingSpace;



    for(i = 0; i < blocks; i++)
    {
        int offset = spaces[i]/2;
        int rest = spaces[i] - offset;

        printf(RESET);
        if( free[i] )
            printf(GRN);
        else
            printf(RED);

        while( offset-- ) printf(" ");
        printf("%" PRIu64 , blockSizes[i]);
        while( rest-- ) printf(" ");

        if( i != blocks-1 ){
            printf(RESET);
            printf(YEL "|");
        }
    }
    printf(RESET "\n");
}

void printBlocks()
{
    pthread_mutex_lock(&printingMutex); //MUTEX

	int it = 1;
	area* currentArea = firstArea;
	while( currentArea != NULL )
	{
        char areaStr[30];
        snprintf(areaStr, 30, "AREA: %d SIZE: %d", it, (int)currentArea->size);

        int offset = strlen(areaStr);
        offset = (AREA_WIDTH - offset + 1) / 2;
        int rest = AREA_WIDTH - offset - strlen(areaStr);

        printf(BLU);
        while( offset-- ) printf(" ");
        printf("%s", areaStr);
        while( rest-- ) printf(" ");

        printf(RESET "\n");


		int spaceForText = AREA_WIDTH;
        char str[12];
		block* currentBlock = currentArea->firstBlock;

        int blocks = 0;

        while( currentBlock != NULL ){
                currentBlock = currentBlock->next;
                blocks++;
        }

        currentBlock = currentArea->firstBlock;
        uint64_t blockSizes[blocks];
        bool free[blocks];
        int baseLength[blocks];
        int requiredSpace = blocks - 1;
        int i;
        for( i = 0; currentBlock != NULL; i++ )
        {
            blockSizes[i] = (uint64_t)abs(currentBlock->size);
            free[i] = (currentBlock->size < 0) ? true : false;
            baseLength[i] = numOfDigits(blockSizes[i]);
            requiredSpace += baseLength[i];

            currentBlock = currentBlock->next;
        }

        if( requiredSpace <= AREA_WIDTH ){
            printArea(blocks, blockSizes, free, baseLength);
        }
        else{
            int space = baseLength[0];
            int last = 0;
            for( i = 1; i < blocks; i++ )
            {
                space += baseLength[i]+1;
                if( space > AREA_WIDTH ){
                    printArea(i - last, &blockSizes[last], &free[last], &baseLength[last]);
                    last = i;
                    space = baseLength[i];
                }
            }
            printArea(blocks - last , &blockSizes[last], &free[last], &baseLength[last]);
        }

        currentArea = currentArea->next;
        ++it;
	}

    pthread_mutex_unlock(&printingMutex); //MUTEX
}
