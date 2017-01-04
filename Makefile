CC=gcc
INCLUDE=-I dynmem/
LIBS=-L dynmem/ -Wl,-rpath=dynmem/ -ldynmem

all: test.c
	$(CC) test.c $(LIBS) $(INCLUDE) -o test

clean: 
	rm test

