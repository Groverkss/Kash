#include "libs.h"

void present_dir(void) {
    printf("%s\n", get_path());
}

void change_dir(CVector *args) {
    char *path_name = NULL;

    if (args->used > 1) {
        if (args->vector[1][0] == '~') {
            path_name = malloc(strlen(HOME) + strlen(args->vector[1]));

            if (strlen(args->vector[1]) == 1) {
                sprintf(path_name, "%s", HOME);
            } else {
                sprintf(path_name, "%s%s", HOME, args->vector[1] + 1);
            }
        } else {
            path_name = args->vector[1];
        }

        warning_error_check(chdir(path_name), -1);
    }
}

void echo(CVector *args) {
    for (int i = 1; i < args->used; i++) {
        printf("%s ", args->vector[i]);
    }
    printf("\n");
}

void list_ls(CVector *args) {
    // TODO: Check portability issues. Should be 1, but its doesnt work
    optind = 0; 
    int flag_l = 0, flag_a = 0;
    int opt_ret;

    while((opt_ret = getopt(args->used, args->vector, "al")) != -1) {
        switch (opt_ret) {
            case 'a':
                flag_a = 1;
                break;
            case 'l':
                flag_l = 1;
                break;
            default:
                fprintf(stderr, "Usage: ls [al]... [FILE]\n");
                return;
        }
    }

    printf("%d %d\n", flag_l, flag_a);
}
