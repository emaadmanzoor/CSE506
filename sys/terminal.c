#include <sys/sbunix.h>

// buffer to copy things into from the kernel buffer
char termbuf[200];

int write(int fd, char* buf, uint64_t n) {
  int i;
  if (fd == STDOUT) {
    for ( i=0; buf[ i ] != '\0'; i++ ) {
      if (buf[i] == '%' && buf[i+1] == 'd') {
	printf( buf, (int)n );
	break;
      } else if (buf[i] == '%' && buf[i+1] == 's') {
	printf( buf, (char*)n );
	break;
      } else if (buf[i] == '%' && buf[i+1] == 'c') {
	printf( buf, (char)n );
	break;
      }
    }
    if ( buf[ i ] == '\0' )
      printf( buf );
  }
  return 0;
}

// returns the number of bytes read
int read(int fd, char* buf, int n) {
  int ret = 0, i;
  inputpos = 0;

  if (fd == STDIN) {
    for( i=0; i<200; i++)
      termbuf[ i ] = 0;

    // wait it out, till the data is here
    while ( inputpos < n ) {
    }

    // copying data into the buffer
    for ( i=0; i<n; i++ ) {
      termbuf[ i ] = inputqueue[ i ];
    }

    for( i=0; i < n; i++ ) {
      *(buf + i) = termbuf[ i ];
      ret++;
    }
    *(buf+i) = '\0';
    
  }
  return ret;
}
