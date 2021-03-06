#include <stdbool.h>

static const char *PROGRAM_NAME = "xmod";

enum XMOD_MODE_TYPE { SYMBOLIC_MODE, OCTAL_MODE } XMOD_MODE_TYPE;

enum XMOD_USER_TYPE { USER, GROUP, OTHER, ALL} XMOD_USER_TYPE;

enum XMOD_PERMISSION_OPERATOR_TYPE { SUBTRACT, ADD, ASSIGN} XMOD_PERMISSION_OPERATOR_TYPE;

struct XmodOptions {
    bool changes;
    bool verbose;
    bool recursive;
};

struct XmodPermissions{
    bool read;
    bool write;
    bool execute;
};

struct XmodSymbolicMode{
    enum XMOD_USER_TYPE user_type;
    enum XMOD_PERMISSION_OPERATOR_TYPE permission_operator_type;
    struct XmodPermissions permissions;
};

union XmodMode {
    struct XmodSymbolicMode symbolic_mode;
    char *octal_mode; //might be a struct as well
};

typedef struct XmodCommand {
    char *str;
    enum XMOD_MODE_TYPE mode_type;
    union XmodMode mode;
    struct XmodOptions options;
    char *file_dir;
} XmodCommand;
