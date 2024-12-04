#ifndef SHELLMAIN_H
#define SHELLMAIN_H

// Define Posix to prevent issues in cell machines
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_CMD_LENGTH 512
#define MAX_ARGS 128
#define MAX_HISTORY 20
#define MAX_PATHS 50

// Shared history and path variables
extern char *history[MAX_HISTORY];
extern int history_index;

extern char *path_list[MAX_PATHS];
extern int path_count;

void execute_command(char *cmd);
void handle_redirection(char *cmd);
void handle_pipeline(char *cmd);
void handle_builtin(char *cmd);
void run_interactive_shell();
void run_batch_mode(char *batch_file);

#endif // SHELLMAIN_H
