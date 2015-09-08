#include <stdio.h>
#include <string.h>

void test_strcmp() {
  printf("strcmp tests start.\n");
  if (strcmp("abcd", "abcd") != 0)
      printf("strcmp test 1 failed\n");
  if (strcmp("", "abcd") > 0)
      printf("strcmp test 2 failed\n");
  if (strcmp("abcd", "") < 0)
      printf("strcmp test 3 failed\n");
  if (strcmp("abcd", "abc") < 0)
      printf("strcmp test 4 failed\n");
  if (strcmp("abc", "abcf") > 0)
      printf("strcmp test 5 failed\n");
  if (strcmp("bbc", "abcf") < 0)
      printf("strcmp test 6 failed\n");
  printf("strcmp tests done.\n");
}
