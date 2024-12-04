#include "shellmain.h"
#include "commands.h"

// Shared variables
char *history[MAX_HISTORY] = {NULL};//array to store history
int history_index = 0;

char *path_list[MAX_PATHS] = {NULL};//saves the path of the current shell
int path_count = 0;

void run_interactive_shell() {
    char cmd[MAX_CMD_LENGTH];
    while (1) {
        printf("major2shell> ");
        fflush(stdout);
        if (fgets(cmd, MAX_CMD_LENGTH, stdin) == NULL) {
            break; // End of file or error
        }
        cmd[strcspn(cmd, "\n")] = 0;
        if (strlen(cmd) == 0) {
            continue; // Ignore empty commands
        }

        // Save command to history
        if (history[history_index]) {
            free(history[history_index]);
        }
        history[history_index] = strdup(cmd);
        history_index = (history_index + 1) % MAX_HISTORY;

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
            continue; // Ignore empty lines
        }
        printf("%s\n", cmd); // Echo the command
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
    char *input_file = NULL;
    char *output_file = NULL;
    char *args[MAX_ARGS];
    int arg_count = 0;

    // Parse command for redirection symbols
    char *token = strtok(cmd, " ");
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
    args[arg_count] = NULL;

    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        if (input_file) {
            int input_fd = open(input_file, O_RDONLY);
            if (input_fd == -1) {
                perror("Failed to open input file");
                exit(EXIT_FAILURE);
            }
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_file) {
            int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd == -1) {
                perror("Failed to open output file");
                exit(EXIT_FAILURE);
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        execvp(args[0], args);
        perror("Execution failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        wait(NULL); // Parent process waits
    } else {
        perror("Fork failed");
    }
}

void handle_pipeline(char *cmd) {
    int pipe_fds[2];
    pid_t pid1, pid2;

    char *args1[MAX_ARGS], *args2[MAX_ARGS];
    char *cmd1 = strtok(cmd, "|");
    char *cmd2 = strtok(NULL, "|");

    // Parse both commands
    char *token;
    int i = 0;
    token = strtok(cmd1, " ");
    while (token != NULL) {
        args1[i++] = token;
        token = strtok(NULL, " ");
    }
    args1[i] = NULL;

    i = 0;
    token = strtok(cmd2, " ");
    while (token != NULL) {
        args2[i++] = token;
        token = strtok(NULL, " ");
    }
    args2[i] = NULL;

    if (pipe(pipe_fds) == -1) {
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == 0) {
        // First command: writes to pipe
        close(pipe_fds[0]);
        dup2(pipe_fds[1], STDOUT_FILENO);
        close(pipe_fds[1]);
        execvp(args1[0], args1);
        perror("Execution failed");
        exit(EXIT_FAILURE);
    }

    pid2 = fork();
    if (pid2 == 0) {
        // Second command: reads from pipe
        close(pipe_fds[1]);
        dup2(pipe_fds[0], STDIN_FILENO);
        close(pipe_fds[0]);
        execvp(args2[0], args2);
        perror("Execution failed");
        exit(EXIT_FAILURE);
    }

    close(pipe_fds[0]);
    close(pipe_fds[1]);
    wait(NULL);
    wait(NULL);
}

void handle_builtin(char *cmd) {//function to use the commands required
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
        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            perror("Execution failed");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            wait(NULL);
        } else {
            perror("Fork failed");
        }
    }
}
