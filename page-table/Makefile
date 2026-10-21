CC = gcc -std=c2x 
CFLAGS = -Wall

%.o: %.c config.h libmlpt.a
	$(CC) $(CFLAGS) -c $<

libmlpt.a: mlpt.o mlpt.h
	ar rcs libmlpt.a mlpt.o

program: main.o libmlpt.a
	$(CC) $(CFLAGS) -o program main.o libmlpt.a

clean:
	rm program main.o mlpt.o libmlpt.a

all: program

