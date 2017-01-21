#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct block block;
typedef struct area area;

struct block {
	block* prev;
	block* next;
	size_t size;
	bool free;
};

struct area {
	area* prev;
	area* next;
	block* firstBlock;
	size_t size;
};

//rozmiary w bajtach struktur block i area
static const size_t blockSize = sizeof(block);
static const size_t areaSize = sizeof(area);

block createBlock(size_t size);
void divideBlock(void* ptr, size_t size);
void createArea(void* ptr, size_t asize, size_t bsize);
block* sfree(size_t size);

#endif