/*
** EPITECH PROJECT, 2024
** my_sudo
** File description:
** set_uid
*/

#include "set_uid.h"

static char *separeteline(char *line)
{
    char *tmp = strtok(line, ":");

    for (int i = 0; i < 2; i++)
        tmp = strtok(NULL, ":");
    return strdup(tmp);
}

char *get_uid(char *user)
{
    FILE *file = fopen("/etc/passwd", "r");
    char *line = NULL;
    size_t len = 0;

    if (file == NULL) {
        printf("Error opening file\n");
        return NULL;
    }
    while (getline(&line, &len, file) != -1) {
        if (strstr(line, user) != NULL) {
            fclose(file);
            return separeteline(line);
        }
    }
    return NULL;
}

int set_uid(char *user)
{
    printf("User: %s\n", get_uid(user));
    setuid(atoi(get_uid(user)));
    return 0;
}
