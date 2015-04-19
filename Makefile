GCC_FLAGS=-Wall -D_GNU_SOURCE
CC=gcc
HEADERS = svcs_utils.h svcs_identifiers.h svcs_commands.h svcs_strings.h
OBJ = svcs_utils.o svcs.o


%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(GCC_FLAGS)

svcs: $(OBJ)
	$(CC) -o $@ $^ $(GCC_FLAGS)

clean:
	rm svcs
	rm $(OBJ)
