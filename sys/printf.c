#include <stdarg.h>

#define VIDEO_MEM_ADDR 0xB8000

/*
 * The display is a matrix of 80 X 25
 */
int display_width = 80;
int display_height = 25;

/*
 * x and y coordinate of the cursor currently
 */
int x_pos = 0;
int y_pos = 0;

void putchar( char c ) {
  unsigned short *location;
  location = (unsigned short*) VIDEO_MEM_ADDR + ( display_width *  y_pos + x_pos );

  if ( c == '\n' ) {
    x_pos = 0;
    y_pos++;
    
  } else {
    *location = c;
    x_pos++;
  }

  /*
   * Reached end of line, begin from the next
   */
  if ( x_pos > 80 ) {
    x_pos = 0;
    y_pos++;
  }

}

void putint( int intgr, int base ) {
  char digits_space[] = "0123456789ABCDEF";
  int num_digits = 0;
  char intbuf[ 26 ];

  do {
    intbuf[ num_digits ] = digits_space[ intgr % base ];
    num_digits++;
    intgr /= base;
  } while ( intgr != 0 );

  /*
   * print the buffer backwards
   */
  for( num_digits--; num_digits >= 0; num_digits-- ) {
    putchar( intbuf[ num_digits ] );
  }

}
void printf( char *str, ... ) {
  int i, num_args = 0;
  int formatted = 0;
  //unsigned int *arg_ptr;
  int intgr;
  va_list valist;

  /*
   * Count number of arguments
   */
  for ( i = 0; str[ i ] != '\0'; i++ ) {
    if ( str[ i ] == '%' )
      num_args++;
  }
  va_start( valist, str );

  for( i = 0; str[ i ] != '\0'; i++ ) {
    if ( !formatted ) {
      if ( str[ i ] == '%' )
        formatted = 1;
      else
        putchar( str[ i ] );
    } else {
      if ( str[ i ] == 'd' ) {
        /*
	 * print integer
	*/
        intgr = va_arg( valist, int );
	putint( intgr, 10 );
	formatted = 0;
      } else if( str[ i ] == 'c' ) {
        putchar( va_arg( valist, int ) );
	formatted = 0;
      } else if ( str[ i ] == 'x' || str[ i ] == 'p' ) {
	intgr = (long) va_arg( valist, char* );
	putint( intgr, 16 );
	formatted = 0;
      } else if ( str[ i ] == 's' ) {
	formatted = 0;
      } else {
        // Unsupported format
      }
    }
  }
  va_end( valist );
}

