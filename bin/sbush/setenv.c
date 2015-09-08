#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sbush.h"

void usage_setenv() {
    printf("Invalid command formulation.\n");
    printf("\tUsage: setenv PATH|PS1 <VALUE>\n");
}

void setenv(char *const args[], int argc) {
    if (argc != 3) {
        usage_setenv();
        return;
    }

    int len = strlen(args[2]);
    char *value = malloc(sizeof(char) * (len + 1));
    strcpy(value, args[2]);

    if (strcmp(args[1], "PATH") == 0) {
       if (path != NULL)
           free(path);
       path = value; 
    } else if (strcmp(args[1], "PS1") == 0) {
       if (ps1 != NULL)
           free(ps1);
       ps1 = value;
    } else {
        // TODO: variable is not PATH or PS1
        usage_setenv();
    }
}
