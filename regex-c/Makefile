
CFLAGS = -g -Wall -Wstrict-prototypes -DLISP_DUMP_AS_C
LDFLAGS = 
CC = gcc

OBJ = lisp.o regex.o regex_posix_parse.o regex_posix_dump.o brzozowski.o
RETOOL_OBJ = retool_dump.o retool_equiv.o retool.o

default: retool

check: retool
	@./runtests.sh

clean:
	rm -f *.o retool

retool: $(OBJ) $(RETOOL_OBJ)
	$(CC) -o retool $(OBJ) $(RETOOL_OBJ) $(LDFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

