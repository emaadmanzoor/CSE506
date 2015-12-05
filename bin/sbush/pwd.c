#include <stdlib.h>
#include <stdio.h>
#include "sbush.h"

void usage_pwd() {
    printf("Invalid command formulation.\n");
    printf("\tUsage: pwd\n");
}

void pwd(char *const args[], int argc) {
    char cwd[32];

    if (argc != 1) {
        usage_pwd();
        return;
    }

    getcwd(cwd, 32);
    printf("%s\n", cwd);
}
