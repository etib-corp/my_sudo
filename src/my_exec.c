/*
** EPITECH PROJECT, 2024
** my_sudo
** File description:
** my_exec
*/

#include "my_exec.h"

void my_exec(char *cmd, char **args)
{
    printf("Executiong: %s\n", cmd);
    for (int i = 0; args[i] != NULL; i++)
        printf("%s\n", args[i]);
    execv(cmd, args);
}
