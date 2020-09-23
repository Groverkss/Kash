#include "libs.h"

static int change_dir(CVector *args) {
    char *path_name = NULL;

    int status_code;
    if (args->used > 1) {
        path_name = replace_tilda(args->vector[1]);
        status_code = warning_error_check(chdir(path_name), -1);
    }
    free(path_name);
    return status_code;
}

static int builtin_setenv(CVector *args) {
    char *var, *value;
    if (args->used == 2) {
        var = args->vector[1];
        value = "";
    } else if (args->used == 3) {
        var = args->vector[1];
        value = args->vector[2];
    } else {
        fprintf(stderr, "Usage: setenv var [value]\n");
        return 1;
    }

    return warning_error_check(setenv(var, value, 1), -1); 
}

static int builtin_unsetenv(CVector *args) {
    if (args->used != 2) {
        fprintf(stderr, "Usage: unsetenv var\n");
        return 1;
    }

    return warning_error_check(unsetenv(args->vector[1]), -1);
}

int execute_builtin(CVector *args, bool use_pipe) {
    int status_code = 0;

    int pipefd[2];
    pipe(pipefd);

    /* Open other pipe end as stdout */
    if (use_pipe) {
        int stdout_check = dup2(pipefd[1], STDOUT_FILENO);
        fatal_error_check(stdout_check, -1);
    }

    close(pipefd[1]);

    /* Execute Commands */
    if (!strcmp(args->vector[0], "cd")) {
        status_code = change_dir(args);
    } else if (!strcmp(args->vector[0], "setenv")) {
        status_code = builtin_setenv(args);
    } else if (!strcmp(args->vector[0], "unsetenv")) {
        status_code = builtin_unsetenv(args);
    }

    /* For environment variables testing

    while(environ[envi] != NULL) {
        printf("%s\n", environ[envi++]);
    }

    */

    /* Open other pipe end as stdin */
    int stdin_check = dup2(pipefd[0], STDIN_FILENO);
    fatal_error_check(stdin_check, -1);
    close(pipefd[0]);
}
