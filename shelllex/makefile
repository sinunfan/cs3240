CC= gcc
CFLAGS = -ggdb

lexical: lexical.o lex.yy.o

lexical.o : y.tab.h lexical.c

lex.yy.o : y.tab.h lex.yy.c

lex.yy.c : shell.l
	flex shell.l

run: lexical
	./lexical

