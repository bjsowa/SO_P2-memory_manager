CC=gcc

default: libdynmem.so

libdynmem.so: dynmem.o
	$(CC) -shared -o libdynmem.so dynmem.o

dynmem.o: dynmem.c dynmem.h
	$(CC) -c -fPIC dynmem.c -o dynmem.o
