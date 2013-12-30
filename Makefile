CC = gcc
CFLAGS = -c -ansi -g

all: project3

project3: 	project3.o sl_repository.o rand100.o
		$(CC) -o project3 project3.o sl_repository.o rand100.o

clean:
		rm -f *.o

%.o:	%.c
		$(CC) $(CFLAGS) $*.c
