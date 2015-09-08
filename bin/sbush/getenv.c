#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sbush.h"

void usage_getenv() {
    printf("Invalid command formulation.\n");
    printf("\tUsage: getenv PATH|PS1\n");
}

void getenv(char *const args[], int argc) {
    if (argc != 2) {
        usage_getenv();
        return;
    }

    if (strcmp(args[1], "PATH") == 0) {
       if (path != NULL)
           printf("PATH=%s\n", path);
       else
           printf("PATH=unset\n");
    } else if (strcmp(args[1], "PS1") == 0) {
       if (ps1 != NULL)
           printf("PS1=%s\n", ps1);
       else
           printf("PS1=unset\n");
    } else {
        // TODO: variable is not PATH or PS1
        usage_getenv();
    }
}
