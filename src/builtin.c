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

static int setenv(char *var, char *value) {
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
    }

    /* Open other pipe end as stdin */
    int stdin_check = dup2(pipefd[0], STDIN_FILENO);
    fatal_error_check(stdin_check, -1);
    close(pipefd[0]);
}
