#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "myprint.h"
#include "apue.h"

//define globle char array for store the letters
char letters[52];
char group[11];
char line[80];
int numLines; int numGroups;

//define the function
void fillLetters(char letters[], int k);
void getGroup(char group[], int l, char letters[], int k);
int my_print( char * str, int size);

int main(){
    int numLine, numGroups;
    srand((unsigned)time(NULL));
    numLine = rand()%251+750;
    
    fillLetters(letters, 52);

    while (numLine-- > 0){
        numGroups = rand()%9 + 1;
        while (numGroups-- > 0){
            //get random groups
            getGroup(group, 11, letters, 52);
            //put groups into a line
            sprintf(line, "%s%c", group, numGroups?' ':'\n');
            my_print(line, 80);
            //check the out is right
            //printf("%s", line);
        }
    }
    return 0;
} 

//fill the letters from a-z and A-Z
void fillLetters (char letters[], int k) {

    k = 0;
    
    for (int i = 0; i < 26; i ++){
        letters[k] = 65+i;
        k++;
    }
    for (int j = 0; j < 26; j++){
        letters[k] = 97+j;
        k++;
    }
}

//get random group of letters, make it become a word
void getGroup(char group[], int l, char letters[], int k){

    int numGroup;

    numGroup = rand()%6+1;

    for(int j = 0; j < numGroup + 1; j++){

        if(j != numGroup){
            group[j] = letters[(rand()%52)+1];
        }
        else
        {
            group[j] = '\0';
        }
    }
}