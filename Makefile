GCC_FLAGS=-Wall -D_GNU_SOURCE

default:
	gcc -o svcs ${GCC_FLAGS} svcs.c
