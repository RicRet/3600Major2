#include "shellmain.h"
#include "commands.h"

#ifndef SHELLMAIN_C
#define SHELLMAIN_C

void run_interactive_shell() {
    char cmd[MAX_CMD_LENGTH];
    while (1) {
        printf("major2shell> ");
        fflush(stdout);
        if (fgets(cmd, MAX_CMD_LENGTH, stdin) == NULL) {
            break;
        }
        cmd[strcspn(cmd, "\n")] = 0; 
        if (strlen(cmd) == 0) {
            continue;  //Ignorea empty lines
        }
        // Save command to history
        history[history_index] = strdup(cmd);
        history_index = (history_index + 1) % 20;

        execute_command(cmd);
    }
}

void run_batch_mode(char *batch_file) {
    FILE *file = fopen(batch_file, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", batch_file);
        exit(EXIT_FAILURE);
    }
    char cmd[MAX_CMD_LENGTH];
    while (fgets(cmd, MAX_CMD_LENGTH, file) != NULL) {
        cmd[strcspn(cmd, "\n")] = 0;
        if (strlen(cmd) == 0) {
            continue;
        }
        printf("%s\n", cmd);  // Echo the command from the batch file
        execute_command(cmd);
    }
    fclose(file);
}

void execute_command(char *cmd) {
    if (strchr(cmd, '>') || strchr(cmd, '<')) {
        handle_redirection(cmd);
    } else if (strchr(cmd, '|')) {
        handle_pipeline(cmd);
    } else {
        handle_builtin(cmd);
    }
}

void handle_redirection(char *cmd) {
    // Handle input/output redirection (< and >)
    char *input_file = NULL;
    char *output_file = NULL;
    char *token = strtok(cmd, " ");
    char *args[MAX_ARGS];
    int arg_count = 0;

    // Parse command and detect redirection
    while (token != NULL) {
        if (strcmp(token, "<") == 0) {
            input_file = strtok(NULL, " ");
        } else if (strcmp(token, ">") == 0) {
            output_file = strtok(NULL, " ");
        } else {
            args[arg_count++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;  // Null-terminate the argument list

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        if (input_file) {
            int input_fd = open(input_file, O_RDONLY);
            if (input_fd == -1) {
                perror("Input file not opened opening");
                exit(EXIT_FAILURE);
            }
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_file) {
            int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd == -1) {
                perror("Output file failed to open ");
                exit(EXIT_FAILURE);
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        execvp(args[0], args);
        perror("execution failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        wait(NULL);  // Wait for child to finish
    } else {
        perror("Fork failed");
    }
}

void handle_pipeline(char *cmd) {
    // Handle pipelining (| operator)
    int pipe_fds[2];
    pid_t pid1, pid2;
    char *args1[MAX_ARGS];
    char *args2[MAX_ARGS];
    char *token = strtok(cmd, "|");
    int i = 0;

    // Parse first command before the pipe
    while (token != NULL) {
        if (i == 0) {
            char *arg = strtok(token, " ");
            int j = 0;
            while (arg != NULL) {
                args1[j++] = arg;
                arg = strtok(NULL, " ");
            }
            args1[j] = NULL;
        } else {
            char *arg = strtok(token, " ");
            int j = 0;
            while (arg != NULL) {
                args2[j++] = arg;
                arg = strtok(NULL, " ");
            }
            args2[j] = NULL;
        }
        token = strtok(NULL, "|");
        i++;
    }

    if (pipe(pipe_fds) == -1) {
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == 0) {
        // First child: execute first command and write to pipe
        close(pipe_fds[0]);
        dup2(pipe_fds[1], STDOUT_FILENO);
        close(pipe_fds[1]);
        execvp(args1[0], args1);
        perror("exec failed");
        exit(EXIT_FAILURE);
    } else if (pid1 > 0) {
        pid2 = fork();
        if (pid2 == 0) {
            // Second child: read from pipe and execute second command
            close(pipe_fds[1]);
            dup2(pipe_fds[0], STDIN_FILENO);
            close(pipe_fds[0]);
            execvp(args2[0], args2);
            perror("exec failed");
            exit(EXIT_FAILURE);
        } else if (pid2 > 0) {
            close(pipe_fds[0]);
            close(pipe_fds[1]);
            wait(NULL);  // Wait for both child processes
            wait(NULL);
        } else {
            perror("Fork failed");
        }
    } else {
        perror("Fork failed");
    }
}

void handle_builtin(char *cmd) {
    // Check for built-in commands and handle them accordingly
    char *args[MAX_ARGS];
    char *token = strtok(cmd, " ");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (strcmp(args[0], "cd") == 0) {
        cd_function(args);
    } else if (strcmp(args[0], "exit") == 0) {
        exit_function();
    } else if (strcmp(args[0], "path") == 0) {
        path_function(args);
    } else if (strcmp(args[0], "myhistory") == 0) {
        myhistory_function(args);
    } else {
        // Not a built-in, execute as a normal command
        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            perror("exec failed");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            wait(NULL);
        } else {
            perror("Fork failed");
        }
    }
}

#endif