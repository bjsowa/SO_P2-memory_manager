CC=gcc
INCLUDE=-I dynmem/
LIBS=-L dynmem/ -Wl,-rpath=dynmem/ -ldynmem

default: dynmem test
	$(CC) test.c $(LIBS) $(INCLUDE) -o test

.PHONY: dynmem clean

dynmem:
	cd dynmem; $(MAKE)

test: test.c
	$(CC) test.c $(LIBS) $(INCLUDE) -o test

clean: 
	rm test; \
	cd dynmem; \
	$(MAKE) clean \

