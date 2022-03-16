#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "myprint.h"
#include "apue.h"

//define the function from my print
int my_print( char * str, int size);

int size = 1000;

int main() {

    int numSpace = 0, s = 0, numGroup = 0;
    char buf[size]; 
    char *group;
    char delim[] = " ";
    char line[size];

    //gets input from user
    

    
    while (fgets(buf, size, stdin))
    {
        //check the length of the user input
        for(int i = 0; i < strlen(buf); i++){
            if(buf[i]==' '){
                s++;
            }
        }
        group = strtok(buf, delim);

        while(group != NULL){
            numGroup++;
            if(numSpace < 5){
                if(numGroup == (s+1)){
                    sprintf(line, "%16s", group);
                }else
                {
                    sprintf(line, "%15s%c", group, ',');
                }
                numSpace++;
            }
            if(numSpace == 5){
                sprintf(line, "%15s%c", group, '\n');
                numSpace = 0;
            }
            my_print(line, 80);
            group = strtok(NULL, delim);
        }
    }
    

    return 0;
}