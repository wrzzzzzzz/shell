#include "headers.h"
#include "signal.h"

void ctrlC(int sig) {
    if (sig != SIGINT) {
        return;
    } else if (sig == SIGINT) {
        printf("\nmumsh $ ");
        // exit(0);
    }
    // printf("\nmumsh $ ");
    fflush(stdout);
    // exit(0);
}

void ctrlC_c(int sig) {
    if (sig != SIGINT) {
        return;
    }
    // printf("I'm a child ctrlC!\n");
    printf("\n");
    fflush(stdout);
}

void ctrlC_cc(int sig) {  // no new line
    if (sig != SIGINT) {
        return;
    }
    // fflush(stdout);
}


