#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define MAX_STACK_SIZE 100
#define PATH_MAX 4096

char *dirStack[MAX_STACK_SIZE];
int top = -1;

// Function to push a directory onto the stack
void pushd(const char *dir) {
    if (top == MAX_STACK_SIZE - 1) {
        fprintf(stderr, "Error: directory stack overflow\n");
        return;
    }

    char *cwd = (char *)malloc(PATH_MAX);
    if (getcwd(cwd, PATH_MAX) == NULL) {
        perror("getcwd");
        free(cwd);
        return;
    }

    dirStack[++top] = cwd;

    if (chdir(dir) != 0) {
        perror("chdir");
        free(dirStack[top--]);
        return;
    }

    printf("Changed to directory: %s\n", dir);
}

// Function to pop a directory from the stack
void popd() {
    if (top == -1) {
        fprintf(stderr, "Error: directory stack underflow\n");
        return;
    }

    char *dir = dirStack[top];
    if (chdir(dir) != 0) {
        perror("chdir");
        return;
    }

    printf("Changed to directory: %s\n", dir);
    free(dirStack[top--]);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [pushd <dir> | popd]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "pushd") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Usage: %s pushd <dir>\n", argv[0]);
            return EXIT_FAILURE;
        }
        pushd(argv[2]);
    } else if (strcmp(argv[1], "popd") == 0) {
        popd();
    } else {
        fprintf(stderr, "Usage: %s [pushd <dir> | popd]\n", argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
