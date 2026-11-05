CC = gcc
CFLAGS = -Wall

.PHONY: clean

%.o: %.c tlb.h mlpt.h config.h
	$(CC) $(CFLAGS) -c $< 

program: main.o tlb.o
	$(CC) $(CFLAGS) -o program main.o tlb.o

clean:
	rm main.o tlb.o program