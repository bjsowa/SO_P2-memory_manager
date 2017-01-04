CC=gcc
INCLUDE=-I dynmem/
LIBS=-L dynmem/ -ldynmem

all: test.c
	$(CC) test.c $(INCLUDE) -o test

clean: 
	rm test

