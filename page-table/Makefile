CC = gcc -std=c2x 
CFLAGS = -Wall

.PHONY: all clean

%.o: %.c config.h
	$(CC) $(CFLAGS) -c $<

libmlpt.a: mlpt.o mlpt.h
	ar rcs libmlpt.a mlpt.o

program: main.o libmlpt.a
	$(CC) $(CFLAGS) -o program main.o libmlpt.a

clean:
	rm program main.o mlpt.o libmlpt.a

all: program

