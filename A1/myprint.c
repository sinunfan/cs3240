#include <stdio.h>
#include <string.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <stdlib.h>
#include <unistd.h>
#include "myprint.h"
#include "apue.h"

enum error {OK = 0, error = 1, sizeerror = 2};
char buf[49];
int count = 0;

//this will not over write the txt file
int my_print( char * str, int size){
    
    enum error ture = OK;
    // int fd = open("a1.txt", O_WRONLY | O_APPEND);

    for(int i =0; i < strlen(str); i++){
        
        buf[count] = str[i];
        count++;
        if((count == 49) || (buf[i] == '\n')){
            write(1, buf, strlen(buf));
            memset(buf, 0, strlen(buf));
            count = 0;
        }else{
            write(1, buf, strlen(buf));
            memset(buf, 0, strlen(buf));
            count = 0;
        }
    }
    // close(fd);
    
    return OK;
}
