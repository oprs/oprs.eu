
CFLAGS = -g -Wall -Wstrict-prototypes -DLISP_DUMP_AS_C
LDFLAGS = 
CC = gcc

OBJ = lisp.o regex.o regex_posix_parse.o regex_posix_dump.o brzozowski.o

default: retool

check: retool
	@./runtests.sh

clean:
	rm -f *.o retool

retool: $(OBJ) retool.o
	$(CC) -o retool retool.o $(OBJ) $(LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

