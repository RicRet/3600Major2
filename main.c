#include "shellmain.h"

//If two arguments are inputed then the second one is treated as a file, if only one then it works as a regular shell
int main(int argc, char *argv[]) {
    if (argc == 1) {
        run_interactive_shell();
    } else if (argc == 2) {
        run_batch_mode(argv[1]);
    } else {
        fprintf(stderr, "Provided more arguments than expected\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}
