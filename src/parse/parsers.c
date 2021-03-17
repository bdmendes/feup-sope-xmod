#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../retrieve/retrievers.h"
#include "../util/utils.h"
#include "parsers.h"

/**
 * Assembles, from an octal number, a struct with the file permissions for an
 * user type.
 *
 * @param permissions struct to be filled
 * @param octal_mode octal number representing the permissions
 */
static void assemble_permissions_user(PermissionTypes *permissions,
                                      mode_t octal_mode) {
    permissions->read = octal_mode & 04;
    permissions->write = octal_mode & 02;
    permissions->execute = octal_mode & 01;
}

/**
 * Assembles into a struct all the file permissions for the three user types.
 *
 * @param file_permissions struct to be filled
 * @param octal_mode octal number representing the permissions
 */
static void assemble_permissions(mode_t octal_mode,
                                 FilePermissions *file_permissions) {
    assemble_permissions_user(&file_permissions->other, octal_mode & 0007);
    assemble_permissions_user(&file_permissions->group,
                              (octal_mode & 0070) >> 3);
    assemble_permissions_user(&file_permissions->owner,
                              (octal_mode & 0700) >> 6);
}

/**
 * Adds changes to the existing permissions.
 *
 * @param curr_permissions existing permissions
 * @param changes flags to be added in the current permissions
 */
static void add_permissions(PermissionTypes *curr_permissions,
                            PermissionTypes *changes) {
    curr_permissions->read |= changes->read;
    curr_permissions->write |= changes->write;
    curr_permissions->execute |= changes->execute;
}

/**
 * Removes changes to the existing permissions.
 *
 * @param curr_permissions existing permissions
 * @param changes flags to be removed in the current permissions
 */
static void remove_permissions(PermissionTypes *curr_permissions,
                               const PermissionTypes *changes) {
    if (changes->read)
        curr_permissions->read = false;
    if (changes->write)
        curr_permissions->write = false;
    if (changes->execute)
        curr_permissions->execute = false;
}

/**
 * Updates current permissions according to the operator received.
 *
 * @param curr_permissions existing permissions
 * @param changes flags to modify current permisisons
 * @param operator operator
 */
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

/**
 * Updates existing permissions according to the symbolic mode received.
 *
 * @param symbolic_changes changes to be made in the existing permissions
 * @param permissions current permissions
 */
static void update_permissions(const char symbolic_changes[],
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

/**
 * Parses symbolic mode.
 *
 * @param symbolic_mode symbolic mode
 * @param xmodCommand xmod command arguments
 */
static void parse_symbolic_mode(char *symbolic_mode, XmodCommand *xmodCommand) {
    mode_t curr_mode;
    FileInfo file_info;
    retrieve_file_info(&file_info, xmodCommand->file_dir);
    curr_mode = file_info.octal_mode;

    FilePermissions curr_mode_permissions;
    assemble_permissions(curr_mode, &curr_mode_permissions);

    const char sep[2] = ",";
    char *pos = NULL;
    for (char *i = strtok_r(symbolic_mode, sep, &pos); i != NULL;) {
        update_permissions(i, &curr_mode_permissions);
        i = strtok_r(NULL, sep, &pos);
    }

    xmodCommand->octal_mode = get_octal_mode(&curr_mode_permissions);
}

/**
 * Parses octal mode.
 *
 * @param mode_str octal mode
 * @param xmodCommand xmod command arguments
 */
static void parse_octal_mode(const char *mode_str, XmodCommand *xmodCommand) {
    mode_t mode = strtoul(mode_str, NULL, 8) & 0777;
    xmodCommand->octal_mode = mode;
}

/**
 * Verifies which options are in a xmod command.
 *
 * @param options options
 * @param xmodCommand xmod command arguments
 */
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

    snprintf(xmodCommand->file_dir, sizeof(xmodCommand->file_dir), "%s",
             argv[mode_index + 1]);
    strip_trailing_slashes(xmodCommand->file_dir);

    if (is_number_arg(argv[mode_index])) {
        parse_octal_mode(argv[mode_index], xmodCommand);
    } else {
        parse_symbolic_mode(argv[mode_index], xmodCommand);
    }

    xmodCommand->file_idx = mode_index + 1;
}