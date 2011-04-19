
CFLAGS = -g -Wall -Wstrict-prototypes -DLISP_DUMP_AS_C
LDFLAGS = 
CC = gcc

OBJ = lisp.o regex.o regex_posix_parse.o regex_posix_dump.o brzozowski.o

default: test

clean:
	rm -f *.o test

test: $(OBJ) test.o
	$(CC) -o test test.o $(OBJ) $(LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

