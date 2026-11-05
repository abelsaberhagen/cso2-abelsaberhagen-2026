CC = gcc
CFLAGS = -Wall -Wextra -std=c99

program: tlb.o mlpt.o main.o
	$(CC) $(CFLAGS) tlb.o mlpt.o main.o -o program

tlb.o: tlb.c tlb.h mlpt.h config.h
	$(CC) $(CFLAGS) -c tlb.c

mlpt.o: mlpt.c mlpt.h config.h
	$(CC) $(CFLAGS) -c mlpt.c

main.o: main.c tlb.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f tlb.o mlpt.o main.o program