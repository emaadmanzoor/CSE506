#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sbush.h"

// runs the executable by searching for it
// on the path, returning -1 on failure.
void runcmd(const char* cmd, char *const args[], int argc) {
  int i, path_len, current_path_idx;
  char *search_path;
  char *full_cmd;
  const char* current_search_path;
  char *envp[1] = { NULL };
  char cwd[32] = {0};

  //printf("calling execve for cmd %s\n", cmd);

  // first try running the command as is, in case
  // it is present in the current working directory
  execve(cmd, args, envp);

  // try appending the cwd
  getcwd(cwd, 32);
  if (strlen(cwd) > 0 && cwd[strlen(cwd) - 1] != '/')
    cwd[strlen(cwd) - 1] = '/';
  full_cmd = (char *) malloc(sizeof(char) * (strlen(cmd) + strlen(cwd) + 1));
  strncpy(full_cmd, cwd, strlen(cwd));
  strncpy(full_cmd + strlen(cwd), cmd, strlen(cmd) + 1);
  execve(full_cmd, args, envp);

  // then search on the PATH
  path_len = strlen(path);
  if (path_len == 0) {
    printf("Bad command or filename.\n");
    return;
  }

  search_path = (char *) malloc(sizeof(char) * (path_len + 1));
  strcpy(search_path, path);

  // replace all : with \0 in a copy of PATH
  for (i = 0; i < path_len; i++) {
    if (search_path[i] == ':')
      search_path[i] = '\0';
  }

  current_path_idx = 0;
  current_search_path = search_path;
  while (current_path_idx < path_len) {
    if (strlen(current_search_path) == 0) {
      current_path_idx++;
      current_search_path++;
      continue;
    }

    // create a new command by prepending the search path
    full_cmd = (char *) malloc(sizeof(char) * (strlen(cmd) +
                                               strlen(current_search_path) + 1));
    strncpy(full_cmd, current_search_path, strlen(current_search_path));
    strncpy(full_cmd + strlen(current_search_path), cmd, strlen(cmd) + 1);
    //printf("Current search path: %s\n", current_search_path);
    //printf("Full command: %s\n", full_cmd);

    // execute the command
    execve(full_cmd, args, envp);

    // command failed
    free(full_cmd);

    // increment the path index to point right after the index
    // of the next \0 (next null index + 1)
    while (search_path[current_path_idx++] != '\0');

    // point the current search path to begin right after the next \0
    current_search_path = search_path + current_path_idx;
  }

  // failure
  free(search_path);
  printf("Bad command or filename.\n");
}
