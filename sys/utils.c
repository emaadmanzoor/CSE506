#include <sys/sbunix.h>

int atoi(const char* s) {
  int i, result = 0;
  for (i = 0; i < strlen(s); i++) {
    result = result * 10 + (s[i] - '0'); 
  }
  return result;
}

int pow(int base, int exp) {
  int b = 1, i;
  for (i = 0; i < exp; i++) {
    b *= base;
  }
  return b;
}

int octtodec(int oct) {
  int dec = 0, exp = 0;
  while (oct != 0) {
      dec = dec + pow(8, exp) * (oct % 10);
      exp++;
      oct /= 10;
  }
  return dec;
}

void panic(char *s) {
  printf("panic: %s", s);
  for(;;) {
    __asm__ __volatile__ ("hlt");
  }
}

void memset(void *b, char c, int len) {
  char *p = b;
  while (len > 0) {
    *p = c;
    p++;
    len--;
  }
}

void memcpy(char *dest, char *src, int len) {
  int i;
  for (i = 0; i < len; i++) {
    *(dest + i) = *(src + i);
  }
}
