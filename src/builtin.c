#include "libs.h"

void present_dir(void) {
    printf("%s\n", get_path());
}

static char *replace_tilda(char *path_name) {

    char *actual_path = NULL;

    if (path_name[0] == '~') {
        actual_path = malloc(strlen(HOME) + strlen(path_name)); 

        if (strlen(path_name) == 1) {
            sprintf(actual_path, "%s", HOME);
        } else {
            sprintf(actual_path, "%s%s", HOME, path_name + 1);
        }
    } else {
        actual_path = strdup(path_name); 
    }

    return actual_path;
}

static void print_contents(char *path_name, int flag_l, int flag_a) {
    DIR *dir = opendir(path_name);
    if (dir == NULL) {
        // Throw warning error
        warning_error_check(0, 0);
        return;
    }

    // Do not free this
    struct dirent *curr_file;
    while(curr_file = readdir(dir)) {

        if (!flag_a && curr_file->d_name[0] == '.') {
            continue;
        }

        char *file_path = malloc(strlen(path_name) +
                strlen(curr_file->d_name) + 2); 

        sprintf(file_path, "%s/%s", path_name, curr_file->d_name);
        struct stat file_stat;
        stat(file_path, &file_stat);

        if (flag_l) {
            // TODO: Add extra info for -l
        }

        if (file_stat.st_mode & S_IFDIR) {
            // If DIR, print in blue color
            cprintf(BLUE, "%s", curr_file->d_name);

        } else if (file_stat.st_mode & S_IXUSR) {
            // If EXEC permission, print in green color
            cprintf(GREEN, "%s", curr_file->d_name);

        } else {
            // Normal color
            printf("%s", curr_file->d_name);
        }

        if (flag_l) {
            printf("\n");
        } else {
            printf("  ");
        }
    }

    if (!flag_l) {
        printf("\n");
    }
}

void change_dir(CVector *args) {
    char *path_name = NULL;

    if (args->used > 1) {
        path_name = replace_tilda(args->vector[1]);
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

    // Flag to check if any directories are passed
    int flag_none = 1;
    for (int i = optind; i < args->used; i++) {
        flag_none = 0;
        print_contents(replace_tilda(args->vector[i]), flag_l, flag_a);
    }

    if (flag_none) {
        print_contents(".", flag_l, flag_a);
    }
}
