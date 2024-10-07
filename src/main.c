/*
** EPITECH PROJECT, 2024
** mySudo
** File description:
** main
*/

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
} flags_t;


static flags_t *init_flags(void)
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
    return flags;
}

static flags_t *manage_options(char **argv)
{
    flags_t *flags = init_flags();

    if (flags == NULL)
        return NULL;
    for (int i = 1; argv[i] != NULL; i++) {
        if (strcmp(argv[i], "-u") == 0)
            flags->_user = argv[i + 1];
        if (strcmp(argv[i], "-g") == 0)
            flags->_group = argv[i + 1];
        if (strcmp(argv[i], "-E") == 0)
            flags->_preserve_env = true;
        if (strcmp(argv[i], "-s") == 0)
            flags->_shell = true;
    }
    return flags;
}


static char *separeteLine(char *line)
{
    int size = (strchr(strchr(line, ':') + 1, ':')) - (strchr(line, ':') + 1);
    return strndup(strchr(line, ':') + 1, size);
}

static char *getPassword(char *user)
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
            return separeteLine(line);
        }
    }
    return NULL;
}

static void my_exec(char *cmd, char **args)
{

    printf("Executiong: %s\n", cmd);
    for (int i = 0; args[i] != NULL; i++)
        printf("%s\n", args[i]);
    execv(cmd, args);
}

int try_password(const char *password)
{
    char *buffer = NULL;
    size_t size = 0;

    for (int i = 0; i < 3; i++) {
        printf("Enter your password: ");
        getline(&buffer, &size, stdin);
        printf("\n");
        buffer[strlen(buffer) - 1] = '\0';
        char *newHash = crypt(buffer, password);
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

    while (path != NULL) {
        char *fullPath = malloc(strlen(path) + strlen(cmd) + 2);
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

int check_password(const char *password, char **argv, char **env)
{
    char *path = NULL;

    if (try_password(password) == 0) {
        printf("Error: password incorrect\n");
        return 84;
    }
    path = check_command(argv[1], env);
    if (path == NULL) {
        printf("Error: command not found\n");
        return 84;
    }
    my_exec(path, argv + 1);
}

void disable_echo(void)
{
    struct termios term;

    tcgetattr(0, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &term);
}

int main(int argc, char **argv, char **env)
{
    disable_echo();
    if (argc < 2)
        return 84;
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        printf("usage: ./my_sudo -h\n");
        printf("usage: ./my_sudo [-ugEs] [command [args]]\n");
        return 0;
    }
    flags_t *flags = manage_options(argv);

    flags->_user = flags->_user == NULL ? getenv("USER") : flags->_user;
    char *password = getPassword(flags->_user);
    if (password == NULL) {
        printf("User not found\n");
        return 84;
    }
    check_password(password, argv, env);
    return 0;
}
