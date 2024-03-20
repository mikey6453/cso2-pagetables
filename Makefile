CC=gcc
CFLAGS=-Wall -std=c11
LDFLAGS=
OBJECTS=mlpt.o

all: libmlpt.a

libmlpt.a: $(OBJECTS)
	ar rcs libmlpt.a $(OBJECTS)

mlpt.o: mlpt.c mlpt.h config.h
	$(CC) $(CFLAGS) -c mlpt.c -o mlpt.o

clean:
	rm -f *.o libmlpt.a
