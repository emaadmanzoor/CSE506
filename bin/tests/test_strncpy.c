#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void test_strncpy() {
  printf("strncpy tests start.\n");

  // dest always contains enough space for src
  // len(dest) >= len(src)

  // n <= len(src)
  // n bytes will be copied to dest
  // dest will not be null terminated
  char *dest = (char *) malloc(sizeof(char) * 5);
  dest[strlen("abcd")] = 'E';
  strncpy(dest, "abcd", strlen("abcd"));

  if (dest[strlen("abcd")] != 'E')
      printf("strncpy test 1: not null-terminated, failed\n");

  dest[strlen("abcd")] = '\0';
  if (strcmp(dest, "abcd") != 0)
      printf ("strncpy test 2: bytes copied, failed\n");

  free(dest);

  //  n > len(src)
  //  len(src) bytes will be copied to dest
  //  n - len(src) nulls will be written to dest
  dest = (char *) malloc(sizeof(char) * 5);
  dest[4] = 'E';
  strncpy(dest, "abcd", strlen("abcd") + 1);

  if (dest[4] != '\0')
      printf("strncpy test 3: is null-terminated, failed\n");

  if (strcmp(dest, "abcd") != 0)
      printf ("strncpy test 4: bytes copied, failed\n");

  free(dest);

  printf("strncpy tests done.\n");
}
