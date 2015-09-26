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
  } 
  else if ( c == '%d' ) {
    
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

void printf( char *str ) {
  int i;
  int formatted = 0;
  int *arg_ptr;
  int intgr;
  char intbuf[ 16 ];

  arg_ptr = &str + 1;

  for( i = 0; str[ i ] != '\0'; i++ ) {
    if ( !formatted ) {
      if ( str[ i ] == "%" )
	formatted = 1;
      else
	putchar( str[ i ] );
    } else {
      if ( str[ i ] == 'd' ) {
	// print integer
	intgr = *arg_ptr;
	while ( intgr > 0 ) {
	  
	}
      } else if( str[ i ] == 'c' ) {
	putchar( *arg_ptr );
      } else if ( str[ i ] == 'x' ) {
      } else if ( str[ i ] == 's' ) {
      } else if ( str[ i ] == 'p' ) {
      } else {
        // Unsupported format
      }
    }
  }
}
