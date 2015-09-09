#include <string.h>

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
