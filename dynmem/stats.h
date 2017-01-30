#ifndef STATS_H
#define STATS_H

#include <stdint.h>

void showAddress(void* ptr);
void printStats();
void printBlocks();

extern int areasCreated;
extern uint64_t takenSpace;
extern uint64_t freeSpace;
extern int blocksDivided;
extern int blocksMerged;

#endif
