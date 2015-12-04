#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sbush.h"

// environment variable init
char *path = NULL;
char *ps1 = NULL;

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
  int i, count = 1;

  if( buf[ 0 ] == '\0' )
    return 0;

  for( i = 0; buf[ i ] != '\0'; i++ ) {
    if( buf[ i ] == ' ' )
      count++;
  }
  return count;
}

/*
getargs takes off from where getcmd finished. It looks at buf from the index
returned by getcmd. So, getargs looks only at userargs and returns
an array of strings containing arguments to the command entered by the user.
The cmd is the first element of the args array which is NULL terminated.
*/
char** getargs( char* cmd, char* buf, int *nargs ) {
  char** args;
  int argnum = 1, count = 0, num_args;
  int argc;
  char temptoken[ 30 ];
  char *buf_end;
  int buflen;

  // strip trailing spaces
  while( *buf == ' ' )
    buf++;

  // strip leading spaces
  buflen = strlen(buf);
  buf_end = buf + buflen - 1;
  while (*buf_end == ' ') {
    *buf_end = '\0';
    buf_end--;
  }

  num_args = countargs( buf );
  // 2 extra strings, one for the command and one for the NULL
  argc = num_args + 2;
  args = malloc( argc * sizeof( char* ) );

  // to return for other programs
  *nargs = num_args + 1;

  args[ argc - 1 ] = NULL;
  args[ 0 ] = malloc( ( strlen( cmd ) + 1 )* sizeof( char ) );
  strcpy( args[ 0 ], cmd );

  /*
   * temptoken is the temporary buffer used to inhale a user entered
   * argument which is then used to malloc the right size of memory
   * in argv. After each argument is inhaled, temptoken is null terminated
   * so as to copy all its contents into argv[ argnum ].
   */
  while( *buf != '\0' ) {
    if ( *buf == ' ' ) {
      temptoken[ count ] = '\0';
      args[ argnum ] = malloc( ( count + 1 ) * sizeof( char ) );
      strcpy( args[ argnum ], temptoken );
      argnum++;
      count = 0;
      buf++;
      continue;
    }
    temptoken[ count ] = *buf;
    buf++;
    count++;
  }

  if( count > 0 ) {
      temptoken[ count ] = '\0';
      args[ argnum ] = malloc( ( count + 1 ) * sizeof( char ) );
      strcpy( args[ argnum ], temptoken );
  }

  return args;
}

int main() {
  static char buf[ MAX_BUF_SIZE ];
  static char cmd[ MAX_CMD_LEN ];
  //char buf[ MAX_BUF_SIZE ] = { 0 };
  //char cmd[ MAX_CMD_LEN ] = { 0 };
  char** args;
  int argc;
  int argindex, pid, bgpid;
  int i, buflen;
  int background = 0;

  // default PATH and PS1
  path = (char *) malloc(sizeof(char) * strlen(DEFAULT_PATH) + 1);
  strcpy(path, DEFAULT_PATH);
  ps1 = (char *) malloc(sizeof(char) * strlen(DEFAULT_PS1) + 1);
  strcpy(ps1, DEFAULT_PS1);

  while ( 1 ) {
    // initialise command/arg data structures
    for ( i = 0; i< MAX_BUF_SIZE; i++ )
      buf[ i ] = 0;
    for ( i = 0; i < MAX_CMD_LEN; i++ )
      cmd[ i ] = 0;

    // prompt
    printf( "\n" );
    printf(ps1);
    printf(">");

    read( 0, buf, MAX_BUF_SIZE );
    if( buf[ 0 ] == 0 )
      continue;

    buflen = strlen( buf );
    /*
     * Check if the cmd needs to be run in the background
     */
    if( buf[ buflen - 1 ] == '&' ) {
      background = 1;
      /*
       * This was a space, making it null to terminate the string earlier
       */
      buf [ buflen - 2 ] = '\0';
    }
    argindex = getcmd( buf, cmd );

    if( argindex == 0 )
      continue;

    args = getargs( cmd, &buf[ argindex ], &argc );

    //printf( "Command: %s\n", cmd );
    //for ( i = 0; args[ i ] != NULL; i++ )
    //  printf( "ARGS %d: %s\n", i, args[ i ] );

    /*
     * handle builtins: setenv, cd, clear, exit
     */
    if (strcmp(cmd, "setenv") == 0) {
      setenv(args, argc);
      continue;
    } else if (strcmp(cmd, "getenv") == 0) {
      getenv(args, argc);
      continue;
    } else if (strcmp(cmd, "cd") == 0) {
      cd(args, argc);
      continue;
    } else if (strcmp(cmd, "clear") == 0) {
      clear();
      continue;
    } else if (strcmp(cmd, "exit") == 0) {
      printf( "Exiting sbush.\n" );
      break;
    }

    /*
     * handle executables
     */
    pid = fork();
    if ( pid == 0 ) {
      /*
       * In the child, if background command to be run
       * fork another child and run the command in it.
       */
      if( background ) {
        // running as a background command: &
        bgpid = fork();
        if( bgpid == 0 ) {
          // child
          runcmd(cmd, args, argc);
        } else if ( bgpid > 0) {
          // parent
        } else {
          printf( "Failed to spawn a background process\n" );
        }
      } else {
        // running as a foreground command
        runcmd(cmd, args, argc);
      }
      // child should exit now
      break;
    } else if ( pid > 0 ) {
      // in the parent
      waitpid( pid, 0, 0 );
    } else {
      printf( "Shell procreation failed\n" );
      break;
    }

    for( i = 0; args[ i ] != NULL; i++ )
      free( args[ i ] );
    free( args );
  }

  free( path );
  free( ps1 );
  for( i = 0; args[ i ] != NULL; i++ )
    free( args[ i ] );
  free( args );

  return 0;
}
