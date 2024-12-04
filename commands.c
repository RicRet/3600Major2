#include "commands.h"

#ifndef COMMANDS_C
#define COMMANDS_C

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void cd_function(char **args) {//created by George
    if (args[1] == NULL) {
        fprintf(stderr, "cd: expected argument to \"cd\"\n");
    } else if (chdir(args[1]) != 0) {
        perror("cd");
    }
}

void exit_function(void) {//created by Ricardo
    printf("Exiting the shell...\n");
    exit(EXIT_SUCCESS);
}

void path_function(char **args) {//created by Ismaeil
    if (!args[1]) {
        printf("Path: ");
        for (int i = 0; i < path_count; i++) printf("%s:", path_list[i]);
        printf("\n");
    } else if (strcmp(args[1], "+") == 0 && args[2]) {
        if (path_count < MAX_PATHS) path_list[path_count++] = strdup(args[2]);
    } else if (strcmp(args[1], "-") == 0 && args[2]) {
        for (int i = 0; i < path_count; i++) {
            if (strcmp(path_list[i], args[2]) == 0) {
                free(path_list[i]);
                for (int j = i; j < path_count - 1; j++) path_list[j] = path_list[j + 1];
                path_list[--path_count] = NULL;
                break;
            }
        }
    } else {
        fprintf(stderr, "Usage: path [+/pathname | -/pathname]\n");
    }
}


void myhistory_function(char **args) {//created by Jose
    for (int i = 0; i < 20; i++) {
        if (history[i] != NULL) {
            printf("%d: %s\n", i, history[i]);
        }
    }
}

#endif