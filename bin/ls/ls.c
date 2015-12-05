#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void usage_ls() {
    printf("Invalid command formulation.\n");
    printf("\tUsage: ls [<PATH>]\n");
}

int main(int argc, char* argv[]) {
  int fd;
  char cwd[32];
  char full_path[200];

  if (argc > 2) {
    usage_ls();
    return 0;
  }

  getcwd(cwd, 32);
  if (strlen(cwd) > 0 && cwd[strlen(cwd) - 1] != '/')
    cwd[strlen(cwd) - 1] = '/';

  memset(full_path, 0, 200);
  if (argc > 1) {
    if (argv[1][0] != '/') {
      // relative path
      strncpy(full_path, cwd, strlen(cwd));
      strncpy(full_path + strlen(cwd), argv[1], strlen(argv[1]) + 1);
    } else {
      // absolute
      strncpy(full_path, argv[1] + 1, strlen(argv[1]) + 1);
      full_path[strlen(argv[1]) - 1] = '/';
    }
  } else {
    strncpy(full_path, cwd, strlen(cwd) + 1);
  }

  fd = (int) (uint64_t) opendir(full_path);
  if (fd < 0 && strcmp(full_path, "/") != 0) {
    printf( "Could not open directory (or not a directory)\n" );
    return -1;
  }
  ls(full_path);
  closedir((void*) (uint64_t) fd);

  return 0;
}
