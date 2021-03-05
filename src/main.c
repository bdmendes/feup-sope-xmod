#include <stdio.h>

static const char *PROGRAM_NAME = "xmod";
// Continue with options, perhaps in a header file

int main(int argc, char **argv) {
    for (int i = 0; i < argc; ++i) {
        printf("Argument %d: %s\n", i, argv[i]);
    }
    return 0;
}
