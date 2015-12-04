#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[], char* envp[]) {
  int x, y, ret;
  void *v = malloc(1);
  char buf[5];
  /*  
  __asm__("\
      mov $1031, %r12 \n\
      loop: \n\
        push $0 \n\
        dec %r12 \n\
        jnz loop \n\
      mov $1031, %r12 \n\
      loop2: \n\
        addq $8, %rsp \n\
        dec %r12 \n\
        jnz loop2 \n\
	");*/

  // Read from stdin:
  ret = read( 0, (char*)&buf[0], 5 );
  printf( "%s\n", buf );
  printf( "%d\n", ret );
  yield();
  *(char*) v = 0x42;
  x = fork();
  if (x == 0) {
    // child p->x
    y = fork();
    if (y > 0) {
      // child p->x
      //while (1) {
      //v = malloc(200);
      //yield();
      //free(v);
      //}
    } else {
      // grandchild p->x->y
      exit(1);
    }
  } else {
    // parent p
    y = fork();
    if (y > 0) {
      // parent p
      //while(1) {
      //yield();
      //}
    } else {
      // child p->y
      //exit(1);
    }
  }
  free(v);
  return 1;
}
/*=======
  //while(1) {
  //  yield();
  //}
  int x, y;
  y = 0;
  //char buf[5];
  x = fork();
  if (x==0) {
    printf( "First Child %d\n", getpid() );
    y = fork();
    printf( "First child/Second child %d\n", getpid() );
    //if ( y > 0 ){
    //while(1) {
    //yield();
    //}
    //}
  } else {
    printf( "Parent %d, wait for child\n", getpid() );
    printf( "Parent, my parent: %d\n", getppid() );
    waitpid( x, 0, 0 );
    printf( "Parent %d after waitpid\n", getpid() );
    //y = fork();
    //if ( y > 0 ){
    //while(1) {
      // Read from stdin:
      //ret = read( 0, (char*)&buf[0], 5 );
      //ret++;
      //printf( "%s\n", buf );
      //printf( "%d, ", getpid() );
      //      printf( "%d\n", getppid() );
      //yield();
      //}
    //}
  }
  printf( "returning from %d\n", getpid());
  return y;
>>>>>>> Stashed changes
}
*/
