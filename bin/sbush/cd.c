#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sbush.h"

void usage_cd() {
    printf("Invalid command formulation.\n");
    printf("\tUsage: cd <PATH>\n");
}

void cd(char *const args[], int argc) {
  char full_path[200];
  char cwd[32];

  if (argc != 2) {
      usage_cd();
      return;
  }

  getcwd(cwd, 32);
  if (strlen(cwd) > 0 && cwd[strlen(cwd) - 1] != '/')
    cwd[strlen(cwd) - 1] = '/';

  memset(full_path, 0, 200);
  if (args[1][0] != '/') {
    // relative path
    strncpy(full_path, cwd, strlen(cwd));
    strncpy(full_path + strlen(cwd), args[1], strlen(args[1]) + 1);
  } else {
    // absolute
    strncpy(full_path, args[1] + 1, strlen(args[1]) + 1);
    full_path[strlen(args[1]) - 1] = '/';
  }

  if (chdir(full_path) < 0) {
    printf("cd to path '");
    printf(args[1]);
    printf("' failed\n");
  }
}
