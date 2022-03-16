CC = gcc
#CFLAGS = ggdb -Wall -padantic
CFLAGS = -o -Wall -pedantic


all: generate table

generate : generate.o myprint.o error.o

generate.o : myprint.h 

myprint.o : myprint.h apue.h

error.o : apue.h

table : table.o myprint.o error.o

clean:
	rm -f *.o core generate a1.txt

run: generate table
	./generate | ./table > a1.txt