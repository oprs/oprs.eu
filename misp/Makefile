
CC      = gcc
OBJ     = misp.o
CFLAGS  = -g -O2 -Wall -Wstrict-prototypes -I.
LDFLAGS = 

MIPSEL_CC      = mipsel-linux-gnu-gcc
MIPSEL_LD      = mipsel-linux-gnu-ld
MIPSEL_CFLAGS  = -g -O0
MIPSEL_LDFLAGS = --unresolved-symbols=ignore-all

default: misp toto.so


clean:
	rm -f *.o *.so misp


misp: $(OBJ)
	$(CC) -o misp $(OBJ) $(LDFLAGS)

toto.so: toto.o
	$(MIPSEL_LD) $(MIPSEL_LDFLAGS) -o toto.so toto.o

toto.o: toto.c
	$(MIPSEL_CC) $(MIPSEL_CFLAGS) -o toto.o -c toto.c

%.o : %.c
	$(CC) $(CFLAGS) -c $<

