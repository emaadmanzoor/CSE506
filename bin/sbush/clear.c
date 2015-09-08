/*
 * See: http://www.cprogramming.com/faq/cgi-bin/smartfaq.cgi?answer=1031963460&id=1043284385
 */
#include <stdio.h>
#include "sbush.h"

void clear() {
  int i;
  for (i = 0; i < SCREEN_HEIGHT; i++)
    printf("\n");
}
