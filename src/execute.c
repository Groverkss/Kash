#include "libs.h"

static const int DEFAULT_ELSIZE = 256;

static char *builtin_commands[] = {"cd", "setenv", "unsetenv", "jobs", "kjob",
                                    "fg", "bg", "overkill", "quit"};

static int present_dir(void) {
    printf("%s\n", get_path());
    return 0;
}

static int builtin_echo(CVector *args) {
    for (int i = 1; i < args->used; i++) {
        printf("%s ", args->vector[i]);
    }
    printf("\n");
    return 0;
}

static int process_info(CVector *args) {
    pid_t pid_to_use;

    if (args->used == 1) {
        pid_to_use = getpid();
    } else if (args->used == 2) {
        pid_to_use = atoi(args->vector[1]);
    } 

    CVector *stat_args = get_stat_args(pid_to_use);
    if (!stat_args) {
        fprintf(stderr, "Cannot obtain information for pid %d\n", pid_to_use);
        return 1;
    }

    printf("pid -- %s\n", stat_args->vector[0]);
    printf("Process Status -- %s\n", stat_args->vector[2]);
    printf("memory -- %s\n", stat_args->vector[22]);
    printf("Executable Path -- %s\n", stat_args->vector[stat_args->used - 1]);
    return 0;
}

static void execute_args(CVector *args) {
    int status_code = 1;

    if(args->used != 0) {
        /* Remove & at the end */
        if (!strcmp(args->vector[args->used - 1], "&")) {
            args->used--;
        }

        if (!strcmp(args->vector[0], "pwd")) {
            status_code = present_dir();
        } else if (!strcmp(args->vector[0], "echo")) {
            status_code = builtin_echo(args);
        } else if (!strcmp(args->vector[0], "ls")) {
            status_code = list_ls(args);
        } else if (!strcmp(args->vector[0], "pinfo")){
            status_code = process_info(args);
        } else if (!strcmp(args->vector[0], "nightswatch")) {
            status_code = nightswatch(args);
        } else if (!strcmp(args->vector[0], "history")) {
            status_code = print_history(args);
        } else {
            /* execvp args must be null terminated */
            pbCVector(args, NULL);

            /* Exit directly if fault */
            fatal_error_check(execvp(args->vector[0], args->vector), -1);
        }
    }

    if (status_code == 0) {
        /* Wait requires exit code 3 for success */ 
        exit(3); 
    } else {
        exit(1);
    }
}

int execute_command(CVector *args, bool use_pipe) {
    if (args->used == 0) {
        /* Exit normally */
        return 0;
    }

    int n_builtin = sizeof(builtin_commands) / sizeof(char *);
    for (int i = 0; i < n_builtin; i++) {
        if (!strcmp(args->vector[0], builtin_commands[i])) {
            return execute_builtin(args, use_pipe);
        }
    }

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

    /* Open pipe */
    int pipefd[2];
    pipe(pipefd);

    pid_t child_pid = fork();

    if (!child_pid) {

        /* Restore Default Signal Handlers */
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        /* Close unused end of pipe */
        close(pipefd[0]);

        /* Open other pipe end as stdout */
        if (use_pipe) {
            int stdout_check = dup2(pipefd[1], STDOUT_FILENO);
            fatal_error_check(stdout_check, -1);
        }

        close(pipefd[1]);
        setpgid(0, 0);

        if (flag_bg) {
            // If bg process, change process group id
        }

        execute_args(args);
    } else {
        int status_code = 0;

        /* Close unused end of pipe */
        close(pipefd[1]);

        /* Add process to process list */
        add_pid(child_pid, args->vector[0]);

        if (!flag_bg) {
            int wstatus;

            signal(SIGTTOU, SIG_IGN), signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(STDIN_FILENO, child_pid);

            waitpid(child_pid, &wstatus, WUNTRACED);

            tcsetpgrp(STDIN_FILENO, getpgrp());
            signal(SIGTTOU, SIG_DFL), signal(SIGTTOU, SIG_DFL);

            if (!WIFSTOPPED(wstatus)) {
                remove_pid(child_pid);
            }

        }

        /* Open other pipe end as stdin */
        int stdin_check = dup2(pipefd[0], STDIN_FILENO);
        fatal_error_check(stdin_check, -1);
        close(pipefd[0]);
    }
}
