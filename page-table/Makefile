CC = gcc -std=c2x 
CFLAGS = -Wall

%.o: %.c config.h mlpt.h
	$(CC) $(CFLAGS) -c $<

program: main.o mlpt.o
	$(CC) $(CFLAGS) -o program main.o mlpt.o

clean:
	rm program main.o mlpt.o