#include "libs.h"

void present_dir(void) {
    printf("%s\n", get_path());
}

void change_dir(char **save_args) {
    char *args = strtok_r(NULL, " \t", save_args);

    // TODO: Doesn't exist error check
    if (args) {
        char *path_name = NULL;

        if (args[0] == '~') {
            path_name = malloc(strlen(HOME) + strlen(args));
            sprintf(path_name, "%s%s", HOME, args + 1);
        } else {
            path_name = args;
        }

        warning_error_check(chdir(path_name), -1);
    }
}

void echo(char **save_args) {
    char *args = strtok_r(NULL, " \t", save_args);

    while(args) {
        printf("%s ", args);
        args = strtok_r(NULL, " \t", save_args);
    }

    printf("\n");
}

void list_ls(char **save_args) {
}
