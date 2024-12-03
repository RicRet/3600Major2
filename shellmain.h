#ifndef SHELLMAIN_H
#define SHELLMAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_CMD_LENGTH 512
#define MAX_ARGS 128

extern char *history[20];
extern int history_index;

void execute_command(char *cmd);
void handle_redirection(char *cmd);
void handle_pipeline(char *cmd);
void handle_builtin(char *cmd);
void run_interactive_shell();
void run_batch_mode(char *batch_file);

#endif
