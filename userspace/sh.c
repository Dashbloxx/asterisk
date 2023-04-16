#include "libc/stdio.h"
#include "libc/stdlib.h"
#include "libc/string.h"
#include "libc/unistd.h"
#include "libc/syscall.h"

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    int status;

    while (1) {
        // Print the command prompt
        printf("# ");

        // Read a line of input from the user
        fgets(input, MAX_INPUT_SIZE, stdin);

        // Tokenize the input into individual arguments
        int num_args = 0;
        args[num_args] = strtok(input, " \t\n");
        while (args[num_args] != NULL && num_args < MAX_ARGS - 1) {
            num_args++;
            args[num_args] = strtok(NULL, " \t\n");
        }
        args[num_args] = NULL;

        // If the user entered no command, skip to the next loop iteration
        if (num_args == 0) {
            continue;
        }

        // Handle the "exit" command as a built-in command
        if (strcmp(args[0], "exit") == 0) {
            return 0;
        }

        // // Fork a new process to execute the command
        // pid_t pid = fork();
        // if (pid == 0) {
        //     // Child process: execute the command
        //     char *envp[] = { NULL };
        //     execve(args[0], args, envp);
        //     printf("error with `execve`...\n");
        //     return -1;
        // } else if (pid < 0) {
        //     // Fork failed
        //     printf("error with `fork`...\n");
        //     return -1;
        // } else {
        //     // Parent process: wait for the child to complete
        //     wait(&status);
        // }

        char *envp[] = { NULL };
        execve(args[0], args, envp);
    }

    return 0;
}
