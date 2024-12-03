#include "shellmain.h"

#ifndef MAIN_C
#define MAIN_C

int main(int argc, char *argv[]) {
    if (argc == 1) {
        run_interactive_shell();
    } else if (argc == 2) {
        run_batch_mode(argv[1]);
    } else {
        fprintf("Provided more arguments than expected\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

#endif