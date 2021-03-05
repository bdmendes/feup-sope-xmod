#include <stdbool.h>

enum XMOD_MODE_TYPE { SYMBOLIC_MODE, OCTAL_MODE } XMOD_MODE_TYPE;

union XmodMode {
    char *symbolic_mode;
    char *octal_mode;
};

struct XmodOptions {
    bool changes;
    bool verbose;
    bool recursive;
};

typedef struct XmodCommand {
    char *str;
    enum XMOD_MODE_TYPE mode_type;
    union XmodMode mode;
    struct XmodOptions options;
    char *file_dir;
} XmodCommand;