#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[], char* envp[]) {
  char *a[4] = {"bin/hello", "arg1", "arg2", (char*) NULL};
  char *e[4] = {"env1", "env2", "env3", (char*) NULL};
  execve("bin/hello", a, e);
  return 0;
}
