CC=gcc
FLAGS= -fPIC

default: libdynmem.so

libdynmem.so: dynmem.o structs.o
	$(CC) -shared -o libdynmem.so dynmem.o structs.o

dynmem.o: dynmem.c dynmem.h structs.h
	$(CC) -c $(FLAGS) dynmem.c -o dynmem.o

structs.o: structs.c structs.h
	$(CC) -c $(FLAGS) structs.c -o structs.o

clean:
	rm dynmem.o structs.o libdynmem.so