#include "parsers.h"
#include "retrievers.h"
#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "xmod.h"

static void assign_permissions(XmodPermissionsTypes *permissions,
                               mode_t octal_mode) {
    permissions->read = octal_mode & 04;
    permissions->write = octal_mode & 02;
    permissions->execute = octal_mode & 01;
}

static void assemble_permissions(mode_t octal_mode,
                                 FilePermissions *file_permissions) {
    assign_permissions(&file_permissions->other, octal_mode & 0007);
    assign_permissions(&file_permissions->group, (octal_mode & 0070) >> 3);
    assign_permissions(&file_permissions->user, (octal_mode & 0700) >> 6);
}

static void add_permissions(XmodPermissionsTypes *old_permissions,
                            XmodPermissionsTypes *changes) {
    old_permissions->read |= changes->read;
    old_permissions->write |= changes->write;
    old_permissions->execute |= changes->execute;
}

static void remove_permissions(XmodPermissionsTypes *curr_permissions,
                               const struct XmodPermissionsTypes *changes) {
    if (changes->read)
        curr_permissions->read = false;
    if (changes->write)
        curr_permissions->write = false;
    if (changes->execute)
        curr_permissions->execute = false;
}

static int update_curr_permissions(XmodPermissionsTypes *old_permissions,
                                   XmodPermissionsTypes *new_permissions,
                                   const char operator) {
    switch (operator) {
        case '+':
            add_permissions(old_permissions, new_permissions);
            break;
        case '-':
            remove_permissions(old_permissions, new_permissions);
            break;
        case '=':
            *old_permissions = *new_permissions;
            break;
        default:
            return 1; // invalid input
    }
    return 0;
}

int update_permissions(const char input_symbolic_mode[],
                       FilePermissions *permissions) {
    int operator_index =
        (input_symbolic_mode[1] == '+' || input_symbolic_mode[1] == '-' ||
         input_symbolic_mode[1] == '=')
            ? 1
            : 0;

    XmodPermissionsTypes input_permissions;
    input_permissions.read = strchr(input_symbolic_mode, 'r') != NULL;
    input_permissions.write = strchr(input_symbolic_mode, 'w') != NULL;
    input_permissions.execute = strchr(input_symbolic_mode, 'x') != NULL;

    switch (input_symbolic_mode[0]) {
        case 'u':
            update_curr_permissions(&permissions->user, &input_permissions,
                                    input_symbolic_mode[operator_index]);
            break;
        case 'g':
            update_curr_permissions(&permissions->group, &input_permissions,
                                    input_symbolic_mode[operator_index]);
            break;
        case 'o':
            update_curr_permissions(&permissions->other, &input_permissions,
                                    input_symbolic_mode[operator_index]);
            break;
        case 'a':
        default:
            update_curr_permissions(&permissions->user, &input_permissions,
                                    input_symbolic_mode[operator_index]);
            update_curr_permissions(&permissions->group, &input_permissions,
                                    input_symbolic_mode[operator_index]);
            update_curr_permissions(&permissions->other, &input_permissions,
                                    input_symbolic_mode[operator_index]);
            break;
    }
    return 0;
}

mode_t get_octal_mode(FilePermissions *permissions) {
    return permissions->user.read * S_IRUSR |
           permissions->user.write * S_IWUSR |
           permissions->user.execute * S_IXUSR |
           permissions->group.read * S_IRGRP |
           permissions->group.write * S_IWGRP |
           permissions->group.execute * S_IXGRP |
           permissions->other.read * S_IROTH |
           permissions->other.write * S_IWOTH |
           permissions->other.execute * S_IXOTH;
}

void get_symbolic_string(mode_t mode, char *string) {
    string[USER * 3 + READ] = mode & S_IRUSR ? 'r' : '-';
    string[USER * 3 + WRITE] = mode & S_IWUSR ? 'w' : '-';
    string[USER * 3 + EXECUTE] = mode & S_IXUSR ? 'x' : '-';

    string[GROUP * 3 + READ] = mode & S_IRGRP ? 'r' : '-';
    string[GROUP * 3 + WRITE] = mode & S_IWGRP ? 'w' : '-';
    string[GROUP * 3 + EXECUTE] = mode & S_IXGRP ? 'x' : '-';

    string[OTHER * 3 + READ] = mode & S_IROTH ? 'r' : '-';
    string[OTHER * 3 + WRITE] = mode & S_IWOTH ? 'w' : '-';
    string[OTHER * 3 + EXECUTE] = mode & S_IXOTH ? 'x' : '-';
}

int parse_symbolic_mode(char *symbolic_mode, XmodCommand *xmodCommand) {
    mode_t curr_mode;
    FileInfo file_info;
    retrieve_file_info(&file_info, xmodCommand->file_dir);
    curr_mode = file_info.octal_mode;

    FilePermissions curr_mode_permissions;
    assemble_permissions(curr_mode, &curr_mode_permissions);

    const char sep[2] = ",";
    for (char *i = strtok(symbolic_mode, sep); i != NULL;) {
        update_permissions(i, &curr_mode_permissions);
        i = strtok(NULL, sep);
    }

    xmodCommand->octal_mode = get_octal_mode(&curr_mode_permissions);
    return 0;
}

static int parse_octal_mode(const char *mode_str, XmodCommand *xmodCommand) {
    char *buf;
    mode_t mode = strtoul(mode_str, &buf, 8) & 0777;
    xmodCommand->octal_mode = mode;
    return 0;
}

static void parse_options(const char *options, XmodCommand *xmodCommand) {
    xmodCommand->options.recursive |= strchr(options, 'R') != NULL;
    xmodCommand->options.verbose |= strchr(options, 'v') != NULL;
    xmodCommand->options.changes |=
        strchr(options, 'c') != NULL && !xmodCommand->options.verbose;
}

int parse(char **argv, XmodCommand *xmodCommand) {
    int mode_index = 1;

    memset(&xmodCommand->options, 0, sizeof(xmodCommand->options));
    while (is_flag_arg(argv[mode_index])) {
        parse_options(argv[mode_index++], xmodCommand);
    }

    // should pass argc to this function to check here
    strcpy(xmodCommand->file_dir, argv[mode_index + 1]);

    if (is_number_arg(argv[mode_index])) {
        parse_octal_mode(argv[mode_index], xmodCommand);
    } else {
        parse_symbolic_mode(argv[mode_index], xmodCommand);
    }

    xmodCommand->file_idx = mode_index + 1;

    return 0;
}