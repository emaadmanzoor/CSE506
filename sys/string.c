#include <sys/sbunix.h>

/*
 * Compares 2 strings
 * Returns:
 *      0 if equal
 *      -1 if s1 < s2 (lexicographically)
 *      1 if s1 > s2 (lexicographically)
 *
 * NOTE: The parameters are const to protect (compile-time)
 * changing the contents of the strings from within the function.
 */
int strcmp(const char* s1, const char* s2) {
    // first compare characters upto length min(len(s1), len(s2))
    for (; *s1 != '\0' && *s2 != '\0'; s1++, s2++) {
        if ((int) (*s1 - *s2) < 0)
            return -1;
        else if ((int) (*s1 - *s2) > 0)
            return 1;
    }
    // all characters were the same so far, look at lengths now

    // s1 is shorter than s2
    if (*s1 == '\0' && *s2 != '\0')
        return -1;
    // s1 is longer than s2
    else if (*s1 != '\0' && *s2 == '\0')
        return 1;
    else
    // s1 and s2 are the same length
        return 0;
}

void strcpy(char *dest, const char *src) {
    while((*dest++ = *src++) != '\0');
}

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

/*
 * Returns the length of the passed string
 *
 * NOTE: The parameters are const to protect (compile-time)
 * changing the contents of the strings from within the function.
 */
int strlen(const char* s) {
    int len = 0;
    while (*s++ != '\0')
        len++;
    return len;
}
