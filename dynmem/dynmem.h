#ifndef DYNMEM_H
#define DYNMEM_H

//szuka najpierw wolnego bloku długości co najmniej size (zob. sfree)
//jeśli taki blok istnieje to go dzieli (zob. divideBlock) i zajmuje
//wpp tworzy nowy obszar długości pierwszej wielokrotności długości 
//strony większej lub równej size, a w nim blok długości co najmniej 
//size (zob. createBlock)
void* malloc(size_t size);

//chyba nic nie trzeba tłumaczyć
void* calloc(size_t count, size_t size);

//to do
void* realloc(void *ptr, size_t size);

//to do
int posix_memalign(void **memptr, size_t alignment, size_t size);

//zmienia typ bloku na wolny i scala go z sąsiednimi wolnymi blokami (zob. mergeFreeBlocks)
void free(void *ptr);

#endif