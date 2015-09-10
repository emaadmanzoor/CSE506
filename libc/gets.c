#include <stdio.h>
#include <stdlib.h>

char* gets( char* buf, int max ) {
  int i, res;
  char c;
  for( i = 0; i < max; i++ ) {
    // Read one character from STDIN
    res = read( 0, &c, 1 );
    if ( res < 1 )
      break;
    buf[ i ] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  // Null terminate the string
  buf[ i ] = '\0';
  return buf;
}
