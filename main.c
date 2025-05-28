#include "shell.h"
#include <signal.h>

// Global flag for graceful exit
volatile sig_atomic_t running = 1;

int main(void) {
    char *input;
    char **tokens;
    int token_count;
    int status;
    
    // Set up signal handling
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);
    
    // Print welcome message
    print_welcome();
    
    // Main shell loop
    while (running) {
        print_prompt();
        
        // Read user input
        input = read_input();
        if (!input) {
            continue;
        }
        
        // Skip empty input
        if (strlen(input) == 0) {
            free(input);
            continue;
        }
        
        // Parse input into tokens
        tokens = parse_input(input, &token_count);
        if (!tokens || token_count == 0) {
            free(input);
            continue;
        }
        
        // Execute command
        status = execute_command(tokens);
        
        // Clean up
        free(input);
        free_tokens(tokens);
        
        // Check if exit was called
        if (status == -1) {
            running = 0;
        }
    }
    
    printf(COLOR_GREEN "Goodbye!\n" COLOR_RESET);
    return 0;
}

void handle_signal(int sig) {
    if (sig == SIGINT) {
        printf("\n");
        print_prompt();
        fflush(stdout);
    } else if (sig == SIGQUIT) {
        printf(COLOR_YELLOW "\nUse 'exit' to quit the shell.\n" COLOR_RESET);
        print_prompt();
        fflush(stdout);
    }
}

void print_welcome(void) {
    printf(COLOR_BLUE "========================================\n");
    printf("       Welcome to SimpleShell!\n");
    printf("========================================\n" COLOR_RESET);
    printf("Type 'help' for available commands.\n");
    printf("Type 'exit' to quit.\n\n");
}

void print_prompt(void) {
    char cwd[MAX_PATH_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf(COLOR_GREEN "%s" COLOR_RESET ":" COLOR_BLUE "%s" COLOR_RESET "$ ", 
               getenv("USER") ? getenv("USER") : "user", cwd);
    } else {
        printf(PROMPT);
    }
    fflush(stdout);
}

char *read_input(void) {
    char *input = malloc(MAX_INPUT_SIZE);
    if (!input) {
        fprintf(stderr, COLOR_RED "Error: Memory allocation failed\n" COLOR_RESET);
        return NULL;
    }
    
    if (!fgets(input, MAX_INPUT_SIZE, stdin)) {
        free(input);
        if (feof(stdin)) {
            printf("\n");
            running = 0;
        }
        return NULL;
    }
    
    // Remove newline character
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    return input;
} 