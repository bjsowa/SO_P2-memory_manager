#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct block block;
typedef struct area area;

struct block {
	block* prev;
	block* next;
	ssize_t size;
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


void divideBlock(void* ptr, size_t size);

//tworzy strukturę obszaru i umieszcza go na początku obszaru wskazanego przez ptr
//za strukturą umieszcza blok długości co najmniej bsize, z tym że
//tworzy blok długości bsize a za nim blok pusty dopełniający obszar, jeśli to możliwe
//wpp po prostu tworzy blok zajmujący cały obszar
//utworzony obszar dodaje do końca listy dwukierunkowej
void createArea(void* ptr, size_t asize, size_t bsize);

//przeszukuje obszary w poszukiwaniu wolnego bloku długości co najmniej size
//jeśli taki istnieje, zwraca wskaźnik na niego, wpp zwraca NULL
block* sfree(size_t size);

#endif