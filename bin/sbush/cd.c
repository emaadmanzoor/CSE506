#include <stdlib.h>
#include <stdio.h>
#include "sbush.h"

void usage_cd() {
    printf("Invalid command formulation.\n");
    printf("\tUsage: cd <PATH>\n");
}

void cd(char *const args[], int argc) {
    if (argc != 2) {
        usage_cd();
        return;
    }

    if (chdir(args[1]) < 0)
        printf("cd to path %s failed\n", args[1]);
}
