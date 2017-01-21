CC=gcc
FLAGS= -fPIC

default: libdynmem.so

libdynmem.so: dynmem.o
	$(CC) -shared -o libdynmem.so dynmem.o

dynmem.o: dynmem.c dynmem.h
	$(CC) -c $(FLAGS) dynmem.c -o dynmem.o

clean:
	rm dynmem.o libdynmem.so