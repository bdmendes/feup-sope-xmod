#include "parsers.h"
#include "../retrieve/retrievers.h"
#include "../util/utils.h"
#include <string.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static void assemble_permissions_user(PermissionTypes *permissions,
                                      mode_t octal_mode) {
    permissions->read = octal_mode & 04;
    permissions->write = octal_mode & 02;
    permissions->execute = octal_mode & 01;
}

static void assemble_permissions(mode_t octal_mode,
                                 FilePermissions *file_permissions) {
    assemble_permissions_user(&file_permissions->other, octal_mode & 0007);
    assemble_permissions_user(&file_permissions->group,
                              (octal_mode & 0070) >> 3);
    assemble_permissions_user(&file_permissions->owner,
                              (octal_mode & 0700) >> 6);
}

static void add_permissions(PermissionTypes *curr_permissions,
                            PermissionTypes *changes) {
    curr_permissions->read |= changes->read;
    curr_permissions->write |= changes->write;
    curr_permissions->execute |= changes->execute;
}

static void remove_permissions(PermissionTypes *curr_permissions,
                               const PermissionTypes *changes) {
    if (changes->read)
        curr_permissions->read = false;
    if (changes->write)
        curr_permissions->write = false;
    if (changes->execute)
        curr_permissions->execute = false;
}

static void update_curr_permissions(PermissionTypes *curr_permissions,
                                    PermissionTypes *changes,
                                    const char operator) {
    switch (operator) {
        case '+':
            add_permissions(curr_permissions, changes);
            break;
        case '-':
            remove_permissions(curr_permissions, changes);
            break;
        case '=':
            *curr_permissions = *changes;
            break;
    }
}

void update_permissions(const char symbolic_changes[],
                        FilePermissions *permissions) {
    int operator_index = is_permission_operator(symbolic_changes[0]) ? 0 : 1;

    PermissionTypes input_permissions;
    input_permissions.read = strchr(symbolic_changes, 'r') != NULL;
    input_permissions.write = strchr(symbolic_changes, 'w') != NULL;
    input_permissions.execute = strchr(symbolic_changes, 'x') != NULL;

    switch (symbolic_changes[0]) {
        case 'u':
            update_curr_permissions(&permissions->owner, &input_permissions,
                                    symbolic_changes[operator_index]);
            break;
        case 'g':
            update_curr_permissions(&permissions->group, &input_permissions,
                                    symbolic_changes[operator_index]);
            break;
        case 'o':
            update_curr_permissions(&permissions->other, &input_permissions,
                                    symbolic_changes[operator_index]);
            break;
        case 'a':
        default:
            update_curr_permissions(&permissions->owner, &input_permissions,
                                    symbolic_changes[operator_index]);
            update_curr_permissions(&permissions->group, &input_permissions,
                                    symbolic_changes[operator_index]);
            update_curr_permissions(&permissions->other, &input_permissions,
                                    symbolic_changes[operator_index]);
            break;
    }
}

mode_t get_octal_mode(FilePermissions *permissions) {
    return permissions->owner.read * S_IRUSR |
           permissions->owner.write * S_IWUSR |
           permissions->owner.execute * S_IXUSR |
           permissions->group.read * S_IRGRP |
           permissions->group.write * S_IWGRP |
           permissions->group.execute * S_IXGRP |
           permissions->other.read * S_IROTH |
           permissions->other.write * S_IWOTH |
           permissions->other.execute * S_IXOTH;
}

void parse_symbolic_mode(char *symbolic_mode, XmodCommand *xmodCommand) {
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
}

static void parse_octal_mode(const char *mode_str, XmodCommand *xmodCommand) {
    mode_t mode = strtoul(mode_str, NULL, 8) & 0777;
    xmodCommand->octal_mode = mode;
}

static void parse_options(const char *options, XmodCommand *xmodCommand) {
    xmodCommand->options.recursive |= strchr(options, 'R') != NULL;
    xmodCommand->options.verbose |= strchr(options, 'v') != NULL;
    xmodCommand->options.changes |=
        strchr(options, 'c') != NULL && !xmodCommand->options.verbose;
}

void parse(char **argv, XmodCommand *xmodCommand) {
    int mode_index = 1;

    memset(&xmodCommand->options, 0, sizeof(xmodCommand->options));
    while (is_flag_arg(argv[mode_index])) {
        parse_options(argv[mode_index++], xmodCommand);
    }

    strcpy(xmodCommand->file_dir, argv[mode_index + 1]);

    if (is_number_arg(argv[mode_index])) {
        parse_octal_mode(argv[mode_index], xmodCommand);
    } else {
        parse_symbolic_mode(argv[mode_index], xmodCommand);
    }

    xmodCommand->file_idx = mode_index + 1;
}