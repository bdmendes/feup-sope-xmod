#ifndef SRC_UTIL_UTILS_H_
#define SRC_UTIL_UTILS_H_

#include <stdbool.h>

/**
 * @brief Verifies if the path corresponds to the current or the previous directory.
 * 
 * @param path path
 * @return true, if the path corresponds to the current or the previous directory; false, otherwise
*/
bool is_ref_path(char *path);

/**
 * @brief Verifies if the argument is a xmod option.
 * 
 * @param argument argument of xmod
 * @return true, if the argument is a xmod option; false, otherwise
*/
bool is_flag_arg(char *argument);

/**
 * @brief Verifies if the argument is composed just by digits.
 * 
 * @param argument argument
 * @return true, if the argument is composed just by digits; false, otherwise
*/
bool is_number_arg(char *argument);

/**
 * @brief Verifies if the argument is composed by permissions flags.
 * 
 * @param argument argument
 * @param start_index where the string starts to be processed
 * @return true, if the argument is composed by permissions flags; false, otherwise
*/
bool has_permissions_flags(const char *argument, int start_index);

/**
 * @brief Verifies if the argument is an operator flag.
 * 
 * @param c operator flag
 * @return true, if the argument is an operator flag; false, otherwise
*/
bool is_permission_operator(char c);

/**
 * @brief Verifies if the argument is an user flag.
 * 
 * @param c userflag
 * @return true, if the argument is an user flag; false, otherwise
*/
bool is_user_flag(char c);

/**
 * @brief Verifies if the argument starts with a comma, has two followed commas or ends with a comma.
 * 
 * @param str argument
 * @return true, if the argument starts with a comma, has two followed commas or ends with a comma; false, otherwise
*/
bool has_irregular_coma(const char str[]);

/**
 * @brief Removes redundant path slashes.
 * 
 * @param str path
*/
void strip_trailing_slashes(char str[]);

#endif // SRC_UTIL_UTILS_H_