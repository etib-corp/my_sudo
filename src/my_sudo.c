/*
** EPITECH PROJECT, 2024
** my_sudo
** File description:
** my_sudo
*/

#include "my_sudo.h"
#include "my_exec.h"
#include "set_uid.h"

static char *separeteline(char *line)
{
    int size = (strchr(strchr(line, ':') + 1, ':')) - (strchr(line, ':') + 1);

    return strndup(strchr(line, ':') + 1, size);
}

static char *getpassword(char *user)
{
    FILE *file = fopen("/etc/shadow", "r");
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

int try_password(const char *password)
{
    char *buffer = NULL;
    size_t size = 0;
    char *newHash = NULL;

    for (int i = 0; i < 3; i++) {
        printf("Enter your password: ");
        getline(&buffer, &size, stdin);
        printf("\n");
        buffer[strlen(buffer) - 1] = '\0';
        newHash = crypt(buffer, password);
        if (strcmp(password, newHash) != 0) {
            printf("Sorry, try again.\n");
        } else
            return 1;
    }
    return 0;
}

char *check_command(char *cmd, char **env)
{
    char *paths = getenv("PATH");
    char *path = strtok(paths, ":");
    char *fullPath = NULL;

    while (path != NULL) {
        fullPath = malloc(strlen(path) + strlen(cmd) + 2);
        strcpy(fullPath, path);
        strcat(fullPath, "/");
        strcat(fullPath, cmd);
        if (access(fullPath, X_OK) == 0)
            return fullPath;
        path = strtok(NULL, ":");
        free(fullPath);
    }
    return NULL;
}

int check_password(flags_t *flags, const char *password, char **argv,
    char **env)
{
    char *path = NULL;

    if (try_password(password) == 0) {
        printf("Error: password incorrect\n");
        return 84;
    }
    path = check_command(argv[flags->_index], env);
    set_uid(flags->_user);
    if (path == NULL) {
        printf("Error: command not found\n");
        return 84;
    }
    my_exec(path, argv + flags->_index);
}

int my_sudo(flags_t *flags, char **argv, char **env)
{
    char *password = getpassword(flags->_user);

    if (password == NULL) {
        printf("User not found\n");
        return 84;
    }
    check_password(flags, password, argv, env);
    return 0;
}
