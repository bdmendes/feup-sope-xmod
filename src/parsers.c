#include <stdbool.h>

typedef enum XMOD_MODE_TYPE { SYMBOLIC_MODE, OCTAL_MODE } XMOD_MODE_TYPE;
typedef union XmodMode {
    char *symbolic_mode;
    char *octal_mode;
} XmodMode;

typedef struct XmodOptions {
    bool changes;
    bool verbose;
    bool recursive;
} XmodOptions;

struct XmodCommand {
    char *str;
    XMOD_MODE_TYPE mode_type;
    XmodMode mode;
    XmodOptions options;
    char *file_dir;
};