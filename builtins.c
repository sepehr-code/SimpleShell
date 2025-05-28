#include "shell.h"

// List of built-in commands
static char *builtin_commands[] = {
    "cd",
    "exit",
    "pwd",
    "help"
};

static int (*builtin_functions[])(char **) = {
    &builtin_cd,
    &builtin_exit,
    &builtin_pwd,
    &builtin_help
};

int is_builtin(char *command) {
    if (!command) return 0;
    
    int num_builtins = sizeof(builtin_commands) / sizeof(char *);
    for (int i = 0; i < num_builtins; i++) {
        if (strcmp(command, builtin_commands[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int execute_builtin(char **tokens) {
    if (!tokens || !tokens[0]) return 0;
    
    int num_builtins = sizeof(builtin_commands) / sizeof(char *);
    for (int i = 0; i < num_builtins; i++) {
        if (strcmp(tokens[0], builtin_commands[i]) == 0) {
            return (*builtin_functions[i])(tokens);
        }
    }
    return 0;
}

int builtin_cd(char **tokens) {
    char *target_dir;
    
    if (tokens[1] == NULL) {
        // No argument provided, go to home directory
        target_dir = getenv("HOME");
        if (!target_dir) {
            fprintf(stderr, COLOR_RED "cd: HOME environment variable not set\n" COLOR_RESET);
            return 1;
        }
    } else {
        target_dir = tokens[1];
    }
    
    if (chdir(target_dir) != 0) {
        perror(COLOR_RED "cd" COLOR_RESET);
        return 1;
    }
    
    return 0;
}

int builtin_exit(char **tokens) {
    int exit_code = 0;
    
    if (tokens[1] != NULL) {
        exit_code = atoi(tokens[1]);
    }
    
    printf(COLOR_GREEN "Exiting with code %d...\n" COLOR_RESET, exit_code);
    return -1; // Special return value to indicate exit
}

int builtin_pwd(char **tokens) {
    (void)tokens; // Suppress unused parameter warning
    
    char cwd[MAX_PATH_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
        return 0;
    } else {
        perror(COLOR_RED "pwd" COLOR_RESET);
        return 1;
    }
}

int builtin_help(char **tokens) {
    (void)tokens; // Suppress unused parameter warning
    
    printf(COLOR_BLUE "SimpleShell - Available Commands:\n" COLOR_RESET);
    printf("=====================================\n\n");
    
    printf(COLOR_GREEN "Built-in Commands:\n" COLOR_RESET);
    printf("  cd [directory]    - Change directory (no arg = home)\n");
    printf("  pwd              - Print working directory\n");
    printf("  exit [code]      - Exit shell (optional exit code)\n");
    printf("  help             - Show this help message\n\n");
    
    printf(COLOR_GREEN "External Commands:\n" COLOR_RESET);
    printf("  Any Linux command available in PATH (ls, mkdir, cat, etc.)\n\n");
    
    printf(COLOR_GREEN "I/O Redirection:\n" COLOR_RESET);
    printf("  command > file   - Redirect output to file\n");
    printf("  command >> file  - Append output to file\n");
    printf("  command < file   - Redirect input from file\n\n");
    
    printf(COLOR_GREEN "Pipes:\n" COLOR_RESET);
    printf("  cmd1 | cmd2      - Pipe output of cmd1 to input of cmd2\n\n");
    
    printf(COLOR_YELLOW "Examples:\n" COLOR_RESET);
    printf("  ls -la\n");
    printf("  cd /home\n");
    printf("  ls > output.txt\n");
    printf("  cat file.txt | grep pattern\n");
    printf("  mkdir test && cd test\n\n");
    
    return 0;
} 