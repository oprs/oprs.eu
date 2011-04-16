
CFLAGS = -g -Wall -Wstrict-prototypes
LDFLAGS = 
CC = gcc

OBJ = s-expr.o regex.o

default: test

clean:
	rm -f *.o test

test: $(OBJ) test.o
	$(CC) -o test test.o $(OBJ) $(LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

