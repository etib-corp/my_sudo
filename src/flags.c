/*
** EPITECH PROJECT, 2024
** my_sudo
** File description:
** flags
*/

#include "flags.h"

flags_t *init_flags(void)
{
    flags_t *flags = malloc(sizeof(flags_t));

    if (flags == NULL) {
        dprintf(2, "Error: malloc failed\n");
        return NULL;
    }
    flags->_user = NULL;
    flags->_group = NULL;
    flags->_preserve_env = false;
    flags->_shell = false;
    flags->_index = 0;
    return flags;
}

int check_flags(flags_t *flags, char **argv, int *index)
{
    if (strcmp(argv[*index], "-u") == 0) {
        flags->_user = argv[*index + 1];
        *index += 1;
    } else if (strcmp(argv[*index], "-g") == 0) {
        flags->_group = argv[*index + 1];
        *index += 1;
    } else {
        if (strcmp(argv[*index], "-E") == 0) {
            flags->_preserve_env = true;
        } else if (strcmp(argv[*index], "-s") == 0) {
            flags->_shell = true;
        } else {
            flags->_index = *index;
            return 0;
        }
    }
    return 1;
}

void print_flags(flags_t *flags)
{
    printf("User: %s\n", flags->_user);
    printf("Group: %s\n", flags->_group);
    printf("Preserve env: %d\n", flags->_preserve_env);
    printf("Shell: %d\n", flags->_shell);
    printf("Index: %d\n", flags->_index);
}

flags_t *manage_options(char **argv)
{
    flags_t *flags = init_flags();

    if (flags == NULL)
        return NULL;
    for (int i = 1; argv[i] != NULL; i++) {
        if (check_flags(flags, argv, &i) == 0)
            break;
    }
    return flags;
}
