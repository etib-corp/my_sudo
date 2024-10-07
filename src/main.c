/*
** EPITECH PROJECT, 2024
** mySudo
** File description:
** main
*/

#include "my_sudo.h"

void disable_echo(void)
{
    struct termios term;

    tcgetattr(0, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &term);
}

int main(int argc, char **argv, char **env)
{
    flags_t *flags = NULL;

    disable_echo();
    if (argc < 2)
        return 84;
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        printf("usage: ./my_sudo -h\n");
        printf("usage: ./my_sudo [-ugEs] [command [args]]\n");
        return 0;
    }
    flags = manage_options(argv);
    flags->_user = flags->_user == NULL ? getenv("USER") : flags->_user;
    print_flags(flags);
    return my_sudo(flags, argv, env);
}
