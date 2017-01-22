CC=gcc
FLAGS= -fPIC -lm

default: libdynmem.so

libdynmem.so: dynmem.o structs.o stats.o
	$(CC) -shared -o libdynmem.so dynmem.o structs.o stats.o

dynmem.o: dynmem.c dynmem.h structs.h stats.h
	$(CC) -c $(FLAGS) dynmem.c -o dynmem.o

structs.o: structs.c structs.h stats.h
	$(CC) -c $(FLAGS) structs.c -o structs.o

stats.o: stats.c stats.h
	$(CC) -c $(FLAGS) stats.c -o stats.o

clean:
	rm dynmem.o structs.o stats.o libdynmem.so