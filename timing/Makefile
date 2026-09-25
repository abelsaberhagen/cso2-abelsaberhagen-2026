CC = gcc
CFLAGS = -Wall -Og

%.o: %.c
	$(CC) $(CFLAGS) -c $<

gettimings: gettimings.o
	$(CC) $(CFLAGS) -o gettimings gettimings.o 

clean:
	rm gettimings gettimings.o