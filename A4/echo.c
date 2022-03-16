#include <stdio.h>

/* this program echos it's command line arguments to stdout 
* you can compile it and test wild cards and what the shell does before
* executing a program
*/
int main(int argc, char * argv[])
{
 int i;

  for(i=0; i < argc; i++) printf(">%s<\n", argv[i]);


  return 0;
}