#include "libs.h"

void present_dir(void) {
    printf("%s\n", get_path());
}

void change_dir(CVector *args) {
    char *path_name = NULL;

    if (args->used > 1) {
        path_name = replace_tilda(args->vector[1]);
        warning_error_check(chdir(path_name), -1);
    }
    free(path_name);
}

void echo(CVector *args) {
    for (int i = 1; i < args->used; i++) {
        printf("%s ", args->vector[i]);
    }
    printf("\n");
}

void execute_command(CVector *args) {
    int flag_bg = 0;

    // Is it a bg process?
    if (args->used > 1) {
        if (!strcmp(args->vector[args->used - 1], "&")) {
            flag_bg = 1;
        }
    }

    for (int i = 0; i < args->used; i++) {
        args->vector[i] = replace_tilda(args->vector[i]);
    }

    // Arguement list for execvp should be NULL terminated
    if (flag_bg) {
        args->vector[args->used - 1] = NULL;
    } else {
        pbCVector(args, NULL);
    }

    // TODO: Add support for background process
    pid_t child_pid = fork();

    if (!child_pid) {
        execvp(args->vector[0], args->vector);
        exit(0);
    } else {
        wait(NULL);
    }
}
