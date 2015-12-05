#include <stdlib.h>
#include <stdio.h>
#include<string.h>

int atoi(const char* s, int len) {
  int i;
  unsigned int result = 0;
  for (i = 0; i < len; i++) {
    result = result * 10 + (s[i] - '0'); 
  }
  return result;
}
int main(int argc, char* argv[], char* envp[]) {
  unsigned int t = atoi( argv[1], strlen(argv[1]));
  return sleep(t);
}
