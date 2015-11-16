#include <stdarg.h>
#include<sys/sbunix.h>
#define VIDEO_MEM_ADDR P2V(0xB8000)

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
void printf( const char *str, ... ) {
  int i, j, num_args = 0;
  int formatted = 0;
  //unsigned int *arg_ptr;
  int intgr;
  va_list valist;
  char* str_buf;

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
	intgr = (long) va_arg( valist, int );
	putint( intgr, 16 );
	formatted = 0;
      } else if ( str[ i ] == 's' ) {
	str_buf = va_arg( valist, char* );
	for( j = 0; str_buf[ j ] != '\0' ; j++ ) {
	  putchar( str_buf[ j ] );
	}
	formatted = 0;
      } else {
        // Unsupported format, don't want to exit from the kernel for this!
      }
    }
  }
  va_end( valist );
}

/*
 * type 0 = char
 * type 1 = int
 */
void printat( int x, int y, int type, int val ) {
  //va_list args;
  int x_pos_backup, y_pos_back_up;
  x_pos_backup = x_pos;
  x_pos = x;
  y_pos_back_up = y_pos;
  y_pos = y;
  /*
   * Modify x_pos and y_pos, back up their values
   */
  if ( type ) {
    putint( val, 10 );
  } else {
    putchar( val );
  }
  /*
   * Put the real values of x and y back
   */
  x_pos = x_pos_backup;
  y_pos = y_pos_back_up;
}
