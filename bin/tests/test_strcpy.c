#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void test_strcpy() {
  printf("strcpy tests start.\n");

  char *dest = (char *) malloc(sizeof(char) * 5);
  dest[4] = 'E'; // should be overwritten by null
  strcpy(dest, "abcd");
  if (strcmp(dest, "abcd") != 0)
      printf("strcpy test 1 failed\n");
  free(dest);

  printf("strcpy tests done.\n");
}
