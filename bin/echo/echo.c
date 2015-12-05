#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[], char* envp[]) {
  int i;
  for(i=1; i< argc; i++) {
    printf( "%s ", argv[i] );
  }
  printf( "\n" );
}
