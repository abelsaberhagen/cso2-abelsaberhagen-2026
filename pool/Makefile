CC = gcc
CFLAGS = -Wall -fsanitize=thread 

.PHONY: clean

%.o: %.c pool.h
	$(CC) $(CFLAGS) -c $<

program: main.o pool.o
	$(CC) $(CFLAGS) -o program main.o pool.o

clean: 
	rm main.o pool.o program