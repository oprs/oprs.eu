
ULISP_PATH = ./ulisp
ULISP_LIB  = $(ULISP_PATH)/libulisp.a

CFLAGS = -g -Wall -Wstrict-prototypes -I$(ULISP_PATH)
LDFLAGS = 
CC = gcc

OBJ = regex.o regex_posix_parse.o regex_posix_dump.o brzozowski.o
RETOOL_OBJ = retool_dump.o retool_echo.o retool_equiv.o retool_deriv.o retool_match.o retool_dfa.o retool.o

.PHONY : ulisp

default: retool

check: retool
	@./runtests.sh

clean:
	rm -f *.o retool

retool: ulisp $(OBJ) $(RETOOL_OBJ)
	$(CC) -o retool $(OBJ) $(RETOOL_OBJ) $(ULISP_LIB) $(LDFLAGS)

ulisp:
	make -C $(ULISP_PATH) lib

%.o : %.c
	$(CC) $(CFLAGS) -c $<

