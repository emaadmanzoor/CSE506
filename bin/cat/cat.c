#include <stdlib.h>
#include <stdio.h>

int main( int argc, char* argv[] ) {
  int i, fd, num;
  char buf[200];

  for( i = 1; i< argc; i++ ) {
    fd = open( argv[ i ], 0 );
    if( fd < 0 ) {
      printf( "Could not open file\n" );
      exit( 1 );
    }
    num = 0;
    for(; (num = read(fd, buf, 200 )); ) {
      if( num < 0 ) {
	printf( "Could not read\n" );
	exit( 1 );
      }
      write(1, buf, num);
    }
    close( fd );
  }
  exit( 0 );
}
