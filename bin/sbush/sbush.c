#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF_SIZE 200
#define MAX_CMD_LEN 50

/*
 * 5 is the actual number of arguments. Adding one for the NULL termination
 * and one for the command itself.
 */
#define MAX_ARGS 5 + 2
#define MAX_ARG_LEN 30
#define MAX_ENV_VARS 2

/*
 * getcmd takes unprocessed user input (buf) and puts the command in the
 * variable "cmd". The index from which user arguments begin in buf is returned
 * from the function as "index".
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
 * countargs looks at the buffer that has all user args ( user entered command has been
 * removed). It returns the size of the args array. args array's first element is the
 * user entered command, followed by arguments and NULL.
*/
int countargs( char* buf ) {
  int i, count = 0;
  for( i = 0; buf[ i ] != '\0'; i++ ) {
    if( buf[ i ] == ' ' )
      count++;
  }
  return count + 3;
}

/*
getargs takes off from getcmd finished. It looks at buf from the index
returned by getcmd. So, getargs looks only at userargs and returns
an array of strings containing arguments to the command entered by the user.
The cmd is the first element of the args array which is NULL terminated.
*/
char** getargs( char* cmd, char* buf ) {
  char** args;
  int argnum = 1, count = 0, i, num_args;

  num_args = countargs( buf );
  
  args = malloc( num_args * sizeof( char* ) );

  for( i = 0; i < num_args; i++ ) {
    args[ i ] = malloc( MAX_ARG_LEN * sizeof( char ) );
  }

  args[ 0 ] = cmd;

  // Remove trailing spaces
  while( *buf == ' ' )
    buf++;

  while( *buf != '\0' ) {
    if ( *buf == ' ' ) {
      printf( "found a spce\n" );
      argnum++;
      count = 0;
      buf++;
      continue;
    }
    args[ argnum ][ count ] = *buf;
    buf++;
    count++;
  }

  if( count > 0 ) {
    // We found non zero arguments to cmd, set everything after that arg to NUL
    argnum++;
  }

  for ( i = argnum ; i < num_args; i++ )
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

    if( argindex == 0 )
      continue;

    args = getargs( cmd, &buf[ argindex ] );
    printf( "Command: %s\n", cmd );
    for ( i = 0; args[ i ] != NULL; i++ )
      printf( "ARGS %d: %s\n", i, args[ i ] );

    pid = fork();
    if ( pid == 0 ) {
      // printf( "In child..\n" );
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
