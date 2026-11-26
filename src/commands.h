#ifndef COMMANDS_H
#define COMMANDS_H

struct Library {
    float (*e)(int);
    int *(*sort)(int *);
    void *handle;
    int version;
};

int run_commands(struct Library *library, int (*switch_library)(struct Library *));

#endif
