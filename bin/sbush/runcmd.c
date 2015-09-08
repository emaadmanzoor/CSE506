#include <stdio.h>
#include <stdlib.h>
#include "sbush.h"

// runs the executable by searching for it
// on the path, returning -1 on failure.
void runcmd(const char* cmd, char *const args[]) {
  int status;

  // first try running the command as is, in case
  // it is present in the current working directory
  status = execve(cmd, args, NULL);

  // then search on the path 

  // failure 
  printf("Bad command or filename: %d\n", status);
}
