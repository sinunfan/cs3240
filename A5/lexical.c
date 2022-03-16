#include <stdio.h>

#include "y.tab.h"
extern void getNext(enum yytokentype *, char *, int, int *);
#define TEXTSIZE 250

void printToken(enum yytokentype token) {
	static char * tokens[] = {"NoToken", "GREAT", "NEWLINE", "WORD", "GREATGREAT",
	"PIPE", "LESS", "GREATAPERSAND", "GREATGREATAPERSAND", "AMPERSAND" };
	printf("Token %s ", tokens[token-NOTOKEN]);
}


int main(int argc, char * argv[]) {
    char text[TEXTSIZE];
	enum yytokentype token = NOTOKEN;
	int textlength;
	while(token != NEWLINE) {
		getNext(&token, text, TEXTSIZE, &textlength);
		printToken( token );
		printf("token %d, text >%s< \n", token, text);
	}
	return 0;
}