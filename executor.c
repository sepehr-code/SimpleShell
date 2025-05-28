#include "shell.h"

int execute_command(char **tokens) {
    if (!tokens || !tokens[0]) {
        return 0;
    }
    
    // Check if it's a built-in command
    if (is_builtin(tokens[0])) {
        return execute_builtin(tokens);
    }
    
    // Check for I/O redirection
    int redir_pos = has_redirection(tokens);
    if (redir_pos > 0) {
        return execute_with_redirection(tokens);
    }
    
    // Check for pipes
    int pipe_pos = has_pipe(tokens);
    if (pipe_pos > 0) {
        return execute_with_pipe(tokens);
    }
    
    // Execute external command
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process
        if (execvp(tokens[0], tokens) == -1) {
            fprintf(stderr, COLOR_RED "SimpleShell: %s: command not found\n" COLOR_RESET, tokens[0]);
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        // Fork failed
        perror(COLOR_RED "fork" COLOR_RESET);
        return 1;
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
    
    return 0;
}

int execute_with_redirection(char **tokens) {
    int redir_pos = has_redirection(tokens);
    if (redir_pos == 0) {
        return execute_command(tokens); // No redirection, execute normally
    }
    
    char *operator = tokens[redir_pos];
    char *filename = tokens[redir_pos + 1];
    
    if (!filename) {
        fprintf(stderr, COLOR_RED "Error: No filename specified for redirection\n" COLOR_RESET);
        return 1;
    }
    
    // Create command array without redirection parts
    char **cmd_tokens = malloc((redir_pos + 1) * sizeof(char *));
    if (!cmd_tokens) {
        fprintf(stderr, COLOR_RED "Error: Memory allocation failed\n" COLOR_RESET);
        return 1;
    }
    
    for (int i = 0; i < redir_pos; i++) {
        cmd_tokens[i] = tokens[i];
    }
    cmd_tokens[redir_pos] = NULL;
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process - handle redirection
        int fd;
        
        if (strcmp(operator, ">") == 0) {
            // Output redirection
            fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror(COLOR_RED "open" COLOR_RESET);
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (strcmp(operator, ">>") == 0) {
            // Append redirection
            fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1) {
                perror(COLOR_RED "open" COLOR_RESET);
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (strcmp(operator, "<") == 0) {
            // Input redirection
            fd = open(filename, O_RDONLY);
            if (fd == -1) {
                perror(COLOR_RED "open" COLOR_RESET);
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        
        // Execute the command
        if (is_builtin(cmd_tokens[0])) {
            exit(execute_builtin(cmd_tokens));
        } else {
            execvp(cmd_tokens[0], cmd_tokens);
            fprintf(stderr, COLOR_RED "SimpleShell: %s: command not found\n" COLOR_RESET, cmd_tokens[0]);
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror(COLOR_RED "fork" COLOR_RESET);
        free(cmd_tokens);
        return 1;
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        free(cmd_tokens);
        return WEXITSTATUS(status);
    }
    
    return 0;
}

int execute_with_pipe(char **tokens) {
    int pipe_pos = has_pipe(tokens);
    if (pipe_pos == 0) {
        return execute_command(tokens); // No pipe, execute normally
    }
    
    // Create arrays for left and right commands
    char **left_cmd = malloc((pipe_pos + 1) * sizeof(char *));
    char **right_cmd = malloc((MAX_TOKENS - pipe_pos) * sizeof(char *));
    
    if (!left_cmd || !right_cmd) {
        fprintf(stderr, COLOR_RED "Error: Memory allocation failed\n" COLOR_RESET);
        free(left_cmd);
        free(right_cmd);
        return 1;
    }
    
    // Split commands at pipe
    for (int i = 0; i < pipe_pos; i++) {
        left_cmd[i] = tokens[i];
    }
    left_cmd[pipe_pos] = NULL;
    
    int j = 0;
    for (int i = pipe_pos + 1; tokens[i] != NULL; i++) {
        right_cmd[j++] = tokens[i];
    }
    right_cmd[j] = NULL;
    
    // Create pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror(COLOR_RED "pipe" COLOR_RESET);
        free(left_cmd);
        free(right_cmd);
        return 1;
    }
    
    pid_t pid1 = fork();
    
    if (pid1 == 0) {
        // First child - left command
        close(pipefd[0]); // Close read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[1]);
        
        if (is_builtin(left_cmd[0])) {
            exit(execute_builtin(left_cmd));
        } else {
            execvp(left_cmd[0], left_cmd);
            fprintf(stderr, COLOR_RED "SimpleShell: %s: command not found\n" COLOR_RESET, left_cmd[0]);
            exit(EXIT_FAILURE);
        }
    } else if (pid1 < 0) {
        perror(COLOR_RED "fork" COLOR_RESET);
        free(left_cmd);
        free(right_cmd);
        return 1;
    }
    
    pid_t pid2 = fork();
    
    if (pid2 == 0) {
        // Second child - right command
        close(pipefd[1]); // Close write end
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin from pipe
        close(pipefd[0]);
        
        if (is_builtin(right_cmd[0])) {
            exit(execute_builtin(right_cmd));
        } else {
            execvp(right_cmd[0], right_cmd);
            fprintf(stderr, COLOR_RED "SimpleShell: %s: command not found\n" COLOR_RESET, right_cmd[0]);
            exit(EXIT_FAILURE);
        }
    } else if (pid2 < 0) {
        perror(COLOR_RED "fork" COLOR_RESET);
        free(left_cmd);
        free(right_cmd);
        return 1;
    }
    
    // Parent process
    close(pipefd[0]);
    close(pipefd[1]);
    
    int status1, status2;
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);
    
    free(left_cmd);
    free(right_cmd);
    
    return WEXITSTATUS(status2); // Return status of last command
} 