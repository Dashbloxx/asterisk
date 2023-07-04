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
        printf("# ");

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            printf("\n");
            continue;
        }
    }

    return 0;
}