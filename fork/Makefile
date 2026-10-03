CC = gcc
CFLAGS = -fsanitize=address  -Og -g

%.o: %.c fork.h
	$(CC) $(CFLAGS) -c $< 

fork: fork.o main.o
	$(CC) $(CFLAGS) -o fork fork.o main.o 

clean:
	rm fork fork.o main.o out.txt err.txt