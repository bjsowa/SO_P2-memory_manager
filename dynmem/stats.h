#ifndef STATS_H
#define STATS_H

#include <stdint.h>

void showAddress(void* ptr);
void printStats();
void printBlocks();

extern int areasCreated;
extern int areasUnmapped;
extern int maxNumAreas;

extern uint64_t takenSpace;
extern uint64_t freeSpace;
extern int blocksDivided;
extern int blocksMerged;

extern uint64_t maxTakenSpace;
extern uint64_t maxFreeSpace;

extern int mallocCalls;
extern int callocCalls;
extern int reallocCalls;
extern int posix_memalignCalls;
extern int freeCalls;

#endif
