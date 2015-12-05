#include "syscall.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int printf(const char *format, ...) {
  va_list val;
  char c;
  int i, formatted = 0, intgr;
  char* str_buf;
  // RELYING ON THE USER SENDING ONLY 1 ARG
  va_start(val, format);
  for( i = 0; format[ i ] != '\0'; i++ ) {
    if ( !formatted ) {
      if ( format[ i ] == '%' )
	formatted = 1;
      else
	continue;
    } else {
      if ( format[ i ] == 'd' ) {
	intgr = (long) va_arg( val, int );
	return __syscall(SYS_fwrite, 1, format, intgr );
      } else if ( format[ i ] == 's' ) {
	str_buf = va_arg( val, char* );
	return __syscall(SYS_fwrite, 1, format, str_buf);
      } else if ( format[ i ] == 'c' ) {
	c = va_arg( val, int );
	return __syscall(SYS_fwrite, 1, format, c);
      }
    }
  }
  return __syscall(SYS_fwrite, 1, format, 0);
}
