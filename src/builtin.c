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

static int builtin_jobs(CVector *args) {
    int index = 1;
    for (process *curr = process_list; curr != NULL; curr = curr->next) {
        char *status;
        int wstatus;

        /* Get status of process */
        CVector *child_stats = get_stat_args(curr->pid);
        char proc_state = child_stats->vector[2][0];

        if (proc_state == 'T') {
            status = "Stopped";
        } else {
            status = "Running";
        }

        printf("[%d] %s %s [%d]\n", index++, status, curr->name, curr->pid);
    }

    return 0;
}

static int builtin_overkill(CVector *args) {
    int status_code = 0; 
    while(process_list) {
        if (kill(process_list->pid, SIGKILL) == -1) {
            status_code = 1;
        }
    }
    return status_code;
}

static int builtin_kjob(CVector *args) {
    if (args->used != 3) {
        fprintf(stderr, "Usage: kjob <job number> <signal number>\n");
        return 1;
    }

    int job_number = atoi(args->vector[1]) - 1;
    int signal_number = atoi(args->vector[2]); 

    process *job_proc = get_by_index(job_number);
    if (job_proc == NULL) {
        fprintf(stderr, "Non-valid Job Number\n");
        return 1;
    }
    return warning_error_check(kill(job_proc->pid, signal_number), -1);
}

static int builtin_bg(CVector *args) {
    if (args->used != 2) {
        fprintf(stderr, "Usage: bg <job number>\n");
        return 1;
    }


    int job_number = atoi(args->vector[1]) - 1;
    process *job_proc = get_by_index(job_number);
    if (job_proc == NULL) {
        fprintf(stderr, "Non-valid Job Number\n");
        return 1;
    }

    return warning_error_check(kill(job_proc->pid, SIGCONT), -1);
}

static int builtin_fg(CVector *args) {
    if (args->used != 2) {
        fprintf(stderr, "Usage: fg <job number>\n");
        return 1;
    }


    int job_number = atoi(args->vector[1]) - 1;
    process *job_proc = get_by_index(job_number);
    if (job_proc == NULL) {
        fprintf(stderr, "Non-valid Job Number\n");
        return 1;
    }
    pid_t child_pid = job_proc->pid;

    int cont_check = warning_error_check(kill(child_pid, SIGCONT), -1);
    if (cont_check) {
        return 1;
    }

    int wstatus;

    signal(SIGTTOU, SIG_IGN), signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(STDIN_FILENO, child_pid);

    waitpid(child_pid, &wstatus, WUNTRACED);

    tcsetpgrp(STDIN_FILENO, getpgrp());
    signal(SIGTTOU, SIG_DFL), signal(SIGTTOU, SIG_DFL);

    if (!WIFSTOPPED(wstatus)) {
        remove_pid(child_pid);
    }

    return 0;
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
    } else if (!strcmp(args->vector[0], "jobs")) {
        status_code = builtin_jobs(args);
    } else if (!strcmp(args->vector[0], "kjob")) {
        status_code = builtin_kjob(args);
    } else if (!strcmp(args->vector[0], "fg")) {
        status_code = builtin_fg(args);
    } else if (!strcmp(args->vector[0], "bg")) {
        status_code = builtin_bg(args);
    } else if (!strcmp(args->vector[0], "overkill")) {
        status_code = builtin_overkill(args);
    } else if (!strcmp(args->vector[0], "quit")) {
        exit(0);
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
