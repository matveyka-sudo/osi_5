#include <stddef.h>

#include "commands.h"
#include "contract.h"

int main(void) {
    struct Library library = {.e = E, .sort = Sort};
    return run_commands(&library, NULL);
}
