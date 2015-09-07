#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void test_strcpy() {
  printf("strcpy tests start.\n");
  
  char *dest = (char *) malloc(sizeof(char) * 5);
  strcpy(dest, "abcd");
  if (strcmp(dest, "abcd") != 0)
      printf("strcpy test 1 failed\n");

  printf("strlen tests done.\n");
}
