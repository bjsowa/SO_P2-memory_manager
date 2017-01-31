CC=gcc
INCLUDE=-I dynmem/
LIBS=-L dynmem/ -Wl,-rpath=dynmem/ -ldynmem -pthread
CFLAGS= -std=c99 
FLAGS= $(LIBS) $(INCLUDE) $(CFLAGS)

all: dynmem test

.PHONY: dynmem clean

dynmem:
	cd dynmem; $(MAKE)

test: test.c
	$(CC) test.c $(FLAGS) -o test

clean: 
	rm test; \
	cd dynmem; \
	$(MAKE) clean \

