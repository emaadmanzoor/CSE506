#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[], char* envp[]) {
  //while(1) {
  //  yield();
  //}
  int x, y, ret;
  char buf[5];
  x = fork();
  if (x==0) {
    y = fork();
    //if ( y > 0 ){
    //while(1) {
    //yield();
    //}
    //}
  } else {
    //y = fork();
    //if ( y > 0 ){
    while(1) {
      // Read from stdin:
      ret = read( 0, (char*)&buf[0], 5 );
      printf( "%s\n", buf );
      printf( "%d\n", ret );
      yield();
    }
    //}
  }
  return y;
}
