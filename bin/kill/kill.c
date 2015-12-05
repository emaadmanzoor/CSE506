#include <stdlib.h>
#include <stdio.h>
#include<string.h>

int atoi(const char* s, int len) {
  int i, result = 0;
  for (i = 0; i < len; i++) {
    result = result * 10 + (s[i] - '0'); 
  }
  return result;
}

int main(int argc, char* argv[], char* envp[]) {
  // assuming syntax like 
  int pid = atoi(argv[2], strlen(argv[2]));
  kill(pid, 9);
}
