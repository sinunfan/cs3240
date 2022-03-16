CC = gcc
CFLAGE = -o wall -pedantic


all: Createfiles Searchfiles
	@echo '\n'Check the readme file first!!!

Createfiles: Createfiles.o

Searchfiles: Searchfiles.o

Searchfiles.o: Searchfiles.h

createfiles: Createfiles
	./Createfiles

searchfiles: Searchfiles
	./Searchfiles


clean:
	rm -f *.o core Createfiles CityAndPosition.txt CityAndZip.txt temp.txt ZipAndPosition.txt