#include "shell.h"

char **parse_input(char *input, int *token_count) {
    char **tokens = malloc(MAX_TOKENS * sizeof(char *));
    if (!tokens) {
        fprintf(stderr, COLOR_RED "Error: Memory allocation failed\n" COLOR_RESET);
        return NULL;
    }
    
    char *token;
    int count = 0;
    
    // Create a copy of input for strtok (which modifies the string)
    char *input_copy = strdup(input);
    if (!input_copy) {
        free(tokens);
        return NULL;
    }
    
    // Tokenize the input
    token = strtok(input_copy, " \t\n");
    while (token != NULL && count < MAX_TOKENS - 1) {
        tokens[count] = strdup(token);
        if (!tokens[count]) {
            // Clean up on allocation failure
            for (int i = 0; i < count; i++) {
                free(tokens[i]);
            }
            free(tokens);
            free(input_copy);
            return NULL;
        }
        count++;
        token = strtok(NULL, " \t\n");
    }
    
    tokens[count] = NULL; // Null-terminate the array
    *token_count = count;
    
    free(input_copy);
    return tokens;
}

void free_tokens(char **tokens) {
    if (!tokens) return;
    
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

int has_redirection(char **tokens) {
    if (!tokens) return 0;
    
    for (int i = 0; tokens[i] != NULL; i++) {
        if (strcmp(tokens[i], ">") == 0 || 
            strcmp(tokens[i], "<") == 0 || 
            strcmp(tokens[i], ">>") == 0) {
            return i; // Return position of redirection operator
        }
    }
    return 0; // No redirection found
}

int has_pipe(char **tokens) {
    if (!tokens) return 0;
    
    for (int i = 0; tokens[i] != NULL; i++) {
        if (strcmp(tokens[i], "|") == 0) {
            return i; // Return position of pipe operator
        }
    }
    return 0; // No pipe found
} 