#ifndef SHELL_H
#define SHELL_H

#define _POSIX_C_SOURCE 200809L  // Enable POSIX functions like strdup

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

// Constants
#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS 64
#define MAX_PATH_SIZE 256
#define PROMPT "SimpleShell> "

// Color codes for better UX
#define COLOR_RESET "\033[0m"
#define COLOR_BLUE "\033[34m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_YELLOW "\033[33m"

// Function declarations

// Parser functions
char **parse_input(char *input, int *token_count);
void free_tokens(char **tokens);
int has_redirection(char **tokens);
int has_pipe(char **tokens);

// Executor functions
int execute_command(char **tokens);
int execute_with_redirection(char **tokens);
int execute_with_pipe(char **tokens);

// Built-in command functions
int is_builtin(char *command);
int execute_builtin(char **tokens);
int builtin_cd(char **tokens);
int builtin_exit(char **tokens);
int builtin_pwd(char **tokens);
int builtin_help(char **tokens);

// Utility functions
void print_prompt(void);
void print_welcome(void);
char *read_input(void);
void handle_signal(int sig);

#endif // SHELL_H 