#include <string.h>

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
    // first compare characters upto lenght min(len(s1), len(s2))
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
