CC=gcc
CFLAGS=-Wall -Wextra -std=c11
SRC=src/*.c
INC=-Iinclude

metro: $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(INC) -o metro

clean:
	rm -f metro