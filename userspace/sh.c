#include "libc/syscall.h"
#include "libc/syscalltable.h"
#include "libc/stdio.h"
#include "libc/string.h"
#include "libc/unistd.h"

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

int exec(char *args[]);

int main() {
    char input[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGUMENTS];
    int arg_count, i;

    while (1) {
        fflush(stdout);
        printf("# ");
        fgets(input, MAX_COMMAND_LENGTH, stdin);

        // Replace '\n' character with NULL terminator
        input[strlen(input)-1] = '\0';

        // Parse input into arguments
        arg_count = 0;
        args[arg_count] = strtok(input, " ");
        while (args[arg_count] != NULL && arg_count < MAX_ARGUMENTS-1) {
            arg_count++;
            args[arg_count] = strtok(NULL, " ");
        }

        // Check for exit command
        if (strcmp(args[0], "exit") == 0) {
            return 0;
        }

        // Execute command
        exec(args);
    }

    return 0;
}

int exec(char *args[]) {
    execute(args[0], args, NULL);
    return 0;
}