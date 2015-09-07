#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF_SIZE 200
#define MAX_CMD_LEN 50
/* 5 is the actual number of arguments. Adding one for the NULL termination
 * and one for the command itself.
 */
#define MAX_ARGS 5 + 2
#define MAX_ARG_LEN 30
#define MAX_ENV_VARS 2

/*
getcmd takes unprocessed user input (buf) and puts the command in the
variable "cmd". The index from which user arguments begin in buf is returned
from the function as "index".
*/
int getcmd( char* buf, char* cmd ) {
  int index = 0;

  // Remove initial whitespaces
  while( *buf == ' ' )
    buf++;
  
  while( *buf != '\0' ) {
    if ( *buf == ' ' ) {
      break;
    }
    cmd[index] = *buf;
    index++;
    buf++;    
  }
  return index;
}

/*
getargs takes off from getcmd finished. It looks at buf from the index
returned by getcmd. So, getargs looks only at userargs and returns
an array of strings containing arguments to the command entered by the user.
The cmd is the first element of the args array which is NULL terminated.
*/
char** getargs( char* cmd, char* buf ) {
  char** args;
  int argnum = 1, count = 0, i;
  args = malloc( MAX_ARGS * sizeof( char* ) );

  for( i = 0; i < MAX_ARGS; i++ ) {
    args[ i ] = malloc( MAX_ARG_LEN * sizeof( char ) );
  }

  args[ 0 ] = cmd;
  for( i = 0; cmd[ i ] != '\0' ; i++ )
    args[ 0 ][ i ] = cmd[ i ];
  
  while( *buf == ' ' )
    buf++;

  while( *buf != '\0' ) {
    if ( *buf == ' ' ) {
      argnum++;
      count = 0;
    }
    args[ argnum ][ count ] = *buf;
    buf++;
    count++;
  }
  for ( i = argnum + 1 ; i < MAX_ARGS; i++ )
  args[ i ] = NULL;
  
  return args;
}

int main() {
  char buf[ MAX_BUF_SIZE ] = { 0 };
  char cmd[ MAX_CMD_LEN ] = { 0 };
  char** args;
  int argindex, pid;
  int i, status;

  while ( 1 ) {    
    printf( "\nsbush>" );
    gets( buf );
    argindex = getcmd( buf, cmd );

    /*
     * User entered empty string, move on
     */
    if( argindex == 0 )
      continue;
    
    args = getargs( cmd, &buf[ argindex ] );

    pid = fork();
    if ( pid == 0 ) {
      // printf( "In child..\n" );
      // TODO: EXEC functionaily
      status = execve( cmd, args, NULL );
      printf( "Exec failed! Returned status %d\n", status );
      exit( status );
    } else if ( pid > 0 ) {
      // printf( "In parent..\n" );
      waitpid( pid, 0, 0 );
      // printf( "Child Exited\n" );
    } else {
      printf( "FORK ERROR, EXITING..\n" );
      exit( 1 );
    }
    // Trying not to use memset so resetting the buffer and cmd manually
    for ( i = 0; i< MAX_BUF_SIZE; i++ )
      buf[ i ] = 0;
    for ( i = 0; i < MAX_CMD_LEN; i++ )
      cmd[ i ] = 0;

  }
  return 0;
}
