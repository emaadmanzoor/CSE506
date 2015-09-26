#include <string.h>

// copies <= n bytes from src into dest.
//
// if src contains < n bytes (not including
// null terminator), len(src) bytes are copied
// and n - len(src) null chars are written.
//
// if src contains >= n bytes (not including
// null terminator), n bytes are copied
// and the null terminator is NOT COPIED.
//
// sample implementation: man strncpy
void strncpy(char *dest, const char *src, int n) {
    int i;

    for (i = 0; i < n && src[i] != '\0'; i++)
       dest[i] = src[i];

    for (; i < n; i++)
       dest[i] = '\0';
}
