CC=gcc
CFLAGS=-Wall -std=c11
LDFLAGS=
OBJECTS=mlpt.o

all: libmlpt.a

libmlpt.a: $(OBJECTS)
	ar rcs libmlpt.a $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o libmlpt.a
