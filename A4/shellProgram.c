#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 

/* 
* this program loop to read program name and 0 or more command line arguments till EOF
*/

int main(int argc, char * argv[]){
    // get the file
    //create the file for the input
    char buf[200];
    pid_t pid;
    int status;

    printf("%%");
    while(fgets(buf, 200, stdin)!= NULL){
        if (buf[strlen(buf)-1] == '\n'){
            buf[strlen(buf)-1] = 0;
        }
        pid = fork();
        // in child pid == 0
        if (pid < 0){
            printf("fork error");
        }else if (pid == 0){
            // get program name and command line arguments
            // call proper exec funtion
            if(execlp(buf,buf,(char *) 0) == -1){
                printf("wrong input!\n");
            }
            return 0;
        }
        // in parent
        // wait for pid then print status
        if((pid = waitpid(pid, &status, 0)) <0 ){
            printf("waitpid error");
        }
        printf("%%");
    }

    return 0;
}