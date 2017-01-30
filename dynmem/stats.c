#include <stdio.h>
#include <string.h>

#include "stats.h"
#include "structs.h"

#define RED_TEXT   "\x1B[31m"
#define RED_BLINK   "\x1B[5;31m"
#define GRN_TEXT   "\x1B[32m"
#define GRN_BLINK   "\x1B[5;32m"
#define RED   "\x1B[7;31m"
#define GRN   "\x1B[7;32m"
#define BLU   "\x1B[7;34m"
#define RESET "\x1B[0m"

int areasCreated = 0;
int blocksDivided = 0;
int blocksMerged = 0;

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
	printf("Blocks Merged: %d\n", blocksMerged);
	printf("Blocks Divided: %d\n", blocksDivided);       
}

void printBlocks()
{
	int i = 1;
	area* currentArea = firstArea;
	while( currentArea != NULL )
	{
        printf(BLU);
        int h=53;
		while(h--)
            printf(" ");
        printf("AREA %d", i);
        h=53;
		while(h--)
            printf(" ");
        printf(RESET "\n");
		int j = 1, spaceForText=100;
        char str[12];
		block* currentBlock = currentArea->firstBlock;

        int allBlocks = 0;

        while( currentBlock != NULL ){
                currentBlock = currentBlock->next;
                allBlocks++;
        }

        currentBlock = currentArea->firstBlock;

		while( currentBlock != NULL )
		{
            printf(RESET);
			if( currentBlock->size < 0 ){
				printf(GRN);
            }
			else{
				printf(RED);
            }
            snprintf(str, 12, " SIZE: %d", abs(currentBlock->size));
            printf(str);
            h=((100*abs(currentBlock->size))/4016)-allBlocks*strlen(str)-1;
            if(spaceForText<h)
                h=spaceForText;
            if(h<0)
                spaceForText-=strlen(str)+1;
            else {
                while(spaceForText>0 && h>0){
                printf(" ");
                --h;
                --spaceForText;
            }}
            allBlocks--;
			currentBlock = currentBlock->next;
			j++;
            if( currentBlock != NULL ){
                printf("|");
            }
		}
        while(spaceForText>0){
            printf(" ");
            spaceForText--;
        }
        printf("|");
        printf(RESET);
		currentArea = currentArea->next;
		i++;
        printf("\n");
	}
}
