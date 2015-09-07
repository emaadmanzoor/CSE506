#include <stdio.h>
#include <string.h>

void test_strlen() {
  printf("strlen tests start.\n");
  if (strlen("abcd") != 4)
      printf("strlen test 1 failed\n");
  if (strlen("") != 0)
      printf("strlen test 2 failed\n");
  if (strlen("124") != 3)
      printf("strlen test 3 failed\n");
  printf("strlen tests done.\n");
}
