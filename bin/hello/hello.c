#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[], char* envp[]) {
  //while(1) {
  //  yield();
  //}
  int x, y;
  x = fork();
  if (x==0) {
    y = fork();
    if ( y > 0 ){
      while(1) {
	yield();
      }
    }
  } else {
    y = fork();
    if ( y > 0 ){
      while(1) {
	yield();
      }
    }
  }
  return y;
}
