GCC_FLAGS=-Wall -D_GNU_SOURCE
CC=gcc

lint:
	$(CC) -o svcs svcs.c $(GCC_FLAGS)

%.c:
	

