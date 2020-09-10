#include "libs.h"

static const int DEFAULT_ELSIZE = 256;

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

void builtin_echo(CVector *args) {
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

    pid_t child_pid = fork();

    if (!child_pid) {
        // Child process
        
        if (flag_bg) {
            // If bg process, change process group id
            setpgid(0, 0);
        }


        fatal_error_check(execvp(args->vector[0], args->vector), -1);
    } else {

        if (!flag_bg) {
            wait(NULL);
        } else {
            /* Format pid + name according to 
             * [pid][null][name]
             */
            char *element = malloc(DEFAULT_ELSIZE);

            // Do memory check

            sprintf(element, "%d$%s", child_pid, args->vector[0]);
            for (int i = 0; i < strlen(element); i++) {
                if (element[i] == '$') {
                    element[i] = '\0';
                    break;
                }
            }

            /* Add pid to pidlist */
            add_to_CVector(pid_list, element);
        }
    }
}

void process_info(CVector *args) {
    pid_t pid_to_use;

    if (args->used == 1) {
        pid_to_use = getpid();
    } else if (args->used == 2) {
        pid_to_use = atoi(args->vector[1]);
    } 

    CVector *stat_args = get_stat_args(pid_to_use);
    if (!stat_args) {
        fprintf(stderr, "Cannot obtain information for pid %d\n", pid_to_use);
        return;
    }

    printf("pid -- %s\n", stat_args->vector[0]);
    printf("Process Status -- %s\n", stat_args->vector[2]);
    printf("memory -- %s\n", stat_args->vector[22]);
    printf("Executable Path -- %s\n", stat_args->vector[stat_args->used - 1]);
}
