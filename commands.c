#include "commands.h"

#ifndef COMMANDS_C
#define COMMANDS_C

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void cd_function(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: expected argument to \"cd\"\n");
    } else if (chdir(args[1]) != 0) {
        perror("cd");
    }
}

void exit_function(void) {
    printf("Exiting the shell...\n");
    exit(EXIT_SUCCESS);
}

void path_function(char **args) {
    // Implementation for the path command (if needed)
    printf("Path command is not yet implemented.\n");
}

void myhistory_function(char **args) {
    for (int i = 0; i < 20; i++) {
        if (history[i] != NULL) {
            printf("%d: %s\n", i, history[i]);
        }
    }
}

#endif