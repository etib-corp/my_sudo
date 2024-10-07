/*
** EPITECH PROJECT, 2024
** my_sudo
** File description:
** flags
*/

#ifndef FLAGS_H_
    #define FLAGS_H_

    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <crypt.h>
    #include <string.h>
    #include <shadow.h>
    #include <termios.h>
    #include <stdbool.h>


typedef struct flags_s {
    char *_user;
    char *_group;
    bool _preserve_env;
    bool _shell;
    int _index;
} flags_t;

flags_t *init_flags(void);

flags_t *manage_options(char **argv);

void print_flags(flags_t *flags);

#endif /* !FLAGS_H_ */
