#include <dlfcn.h>
#include <stdio.h>

#include "commands.h"

static int switch_library(struct Library *library) {
    int version = 1;
    const char *path = "./libfirst." LIB_EXT;
    if (library->version == 1) {
        version = 2;
        path = "./libsecond." LIB_EXT;
    }

    void *handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (handle == NULL) {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }

    float (*e)(int) = (float (*)(int))dlsym(handle, "E");
    int *(*sort)(int *) = (int *(*)(int *))dlsym(handle, "Sort");
    if (e == NULL || sort == NULL) {
        fprintf(stderr, "%s: could not find E or Sort\n", path);
        dlclose(handle);
        return 1;
    }

    if (library->handle != NULL) {
        dlclose(library->handle);
    }
    library->e = e;
    library->sort = sort;
    library->handle = handle;
    library->version = version;
    return 0;
}

int main(void) {
    struct Library library = {0};
    if (switch_library(&library) != 0) {
        return 1;
    }

    int status = run_commands(&library, switch_library);
    dlclose(library.handle);
    return status;
}
