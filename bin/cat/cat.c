#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void usage_cat() {
    printf("Invalid command formulation.\n");
    printf("\tUsage: cat <PATH> [<PATH> <PATH> ...]\n");
}

int main(int argc, char* argv[]) {
  int i, fd, num;
  char buf[75];
  char full_path[200];
  char cwd[32];

  if (argc < 2) {
    usage_cat();
    return 0;
  }

  getcwd(cwd, 32);
  if (strlen(cwd) > 0 && cwd[strlen(cwd) - 1] != '/')
    cwd[strlen(cwd) - 1] = '/';

  for( i = 1; i< argc; i++ ) {
    memset(full_path, 0, 200);
    strncpy(full_path, cwd, strlen(cwd));
    strncpy(full_path + strlen(cwd), argv[i], strlen(argv[i]) + 1);

    fd = open( full_path, 0 );
    if( fd < 0 ) {
      printf( "Could not open file\n" );
      exit( 1 );
    }
    num = 0;
    for(; (num = read(fd, buf, 75)); ) {
      if( num < 0 ) {
        printf( "Could not read\n" );
        exit( 1 );
      }
      write(1, buf, num);
      //printf("\n"); // to avoid overflowing the terminal
    }
    close( fd );
  }
  exit( 0 );
}
