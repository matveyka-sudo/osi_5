#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"

static int read_int(const char *token, int *number) {
    if (token == NULL) {
        return 0;
    }

    char *end;
    errno = 0;
    long value = strtol(token, &end, 10);
    if (end == token || *end != '\0' || errno == ERANGE ||
        value < INT_MIN || value > INT_MAX) {
        return 0;
    }

    *number = (int)value;
    return 1;
}

static int sort_numbers(struct Library *library, char *token, size_t length) {
    const char *spaces = " \t\r\n\v\f";
    /* Each number uses at least one character. Reserve one extra slot for the count. */
    int *array = calloc(length + 1, sizeof(int));
    if (array == NULL) {
        perror("calloc");
        return 1;
    }

    int count = 0;
    while (token != NULL) {
        int number;
        if (count == INT_MAX || !read_int(token, &number)) {
            fprintf(stderr, "Usage: 2 [integer ...]\n");
            free(array);
            return 0;
        }
        ++count;
        array[count] = number;
        token = strtok(NULL, spaces);
    }

    array[0] = count;
    int *sorted = library->sort(array) + 1;
    for (int i = 0; i < count; ++i) {
        if (i > 0) {
            putchar(' ');
        }
        printf("%d", sorted[i]);
    }
    putchar('\n');
    free(array);
    return 0;
}

int run_commands(struct Library *library, int (*switch_library)(struct Library *)) {
    const char *spaces = " \t\r\n\v\f";
    char *line = NULL;
    size_t capacity = 0;
    ssize_t length;
    int status = 0;

    while ((length = getline(&line, &capacity, stdin)) != -1) {
        char *command = strtok(line, spaces);
        if (command == NULL) {
            continue;
        }
        char *arg = strtok(NULL, spaces);

        if (strcmp(command, "0") == 0 && switch_library != NULL && arg == NULL) {
            if (switch_library(library) == 0) {
                printf("Implementation %d\n", library->version);
            }
        } else if (strcmp(command, "1") == 0) {
            int x;
            if (!read_int(arg, &x) || x <= 0 || strtok(NULL, spaces) != NULL) {
                fprintf(stderr, "Usage: 1 positive_integer\n");
                continue;
            }
            printf("%.9g\n", library->e(x));
        } else if (strcmp(command, "2") == 0) {
            status = sort_numbers(library, arg, (size_t)length);
            if (status != 0) {
                break;
            }
        } else {
            fprintf(stderr, "Unknown command\n");
        }
        fflush(stdout);
    }

    if (length == -1 && !feof(stdin)) {
        perror("getline");
        status = 1;
    }
    free(line);
    return status;
}
