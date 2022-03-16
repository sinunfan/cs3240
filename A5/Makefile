CC= gcc
CFLAGS = -ggdb

shell: shell.o lex.yy.o
	
shell.o : y.tab.h shell.c

lex.yy.o : y.tab.h lex.yy.c

lex.yy.c : shell.l
	flex shell.l


run: shell
	@echo please read the readme file first!!!
	./shell
	

