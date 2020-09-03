#include "libs.h"

/**
 * @brief: Returns filetype bit for a file
 * @params: mode: st_mode containing file permission
 * @return: char denoting file type
 */
static int file_type_letter(mode_t mode) {
    char c;
    if (S_ISREG(mode))
        c = '-';
    else if (S_ISDIR(mode))
        c = 'd';
    else if (S_ISBLK(mode))
        c = 'b';
    else if (S_ISCHR(mode))
        c = 'c';
#ifdef S_ISFIFO
    else if (S_ISFIFO(mode))
        c = 'p';
#endif /* S_ISFIFO */
#ifdef S_ISLNK
    else if (S_ISLNK(mode))
        c = 'l';
#endif /* S_ISLNK */
#ifdef S_ISSOCK
    else if (S_ISSOCK(mode))
        c = 's';
#endif /* S_ISSOCK */
#ifdef S_ISDOOR
    /* Solaris 2.6, etc. */
    else if (S_ISDOOR(mode))
        c = 'D';
#endif /* S_ISDOOR */
    else {
        /* Unknown type -- possibly a regular file? */
        c = '?';
    }
    return (c);
}

/**
 * @brief: Returns file perms for a file in ls format
 * @params: mode: st_mode containing file permission
 * @return: 11 bit string containing fileperms
 */
static char *ls_perms(mode_t mode) {
    static const char *rwx[] = {"---", "--x", "-w-", "-wx",
                                "r--", "r-x", "rw-", "rwx"};
    static char bits[11];

    bits[0] = file_type_letter(mode);
    strcpy(&bits[1], rwx[(mode >> 6) & 7]);
    strcpy(&bits[4], rwx[(mode >> 3) & 7]);
    strcpy(&bits[7], rwx[(mode & 7)]);
    if (mode & S_ISUID)
        bits[3] = (mode & S_IXUSR) ? 's' : 'S';
    if (mode & S_ISGID)
        bits[6] = (mode & S_IXGRP) ? 's' : 'l';
    if (mode & S_ISVTX)
        bits[9] = (mode & S_IXOTH) ? 't' : 'T';
    bits[10] = '\0';
    return (bits);
}

/**
 * @brief: Prints extra info for a file for ls
 * @params:
 */
static void extra_info_ls(struct stat *statbuf) {
    printf("%s ", ls_perms(statbuf->st_mode));
    printf("%3.ld ", statbuf->st_nlink);
    printf("%s ", getpwuid(statbuf->st_uid)->pw_name);
    printf("%s ", getgrgid(statbuf->st_gid)->gr_name);
    printf("%13.ld ", statbuf->st_size);

    char date[21];
    strftime(date, 20, "%b %d %H:%M", localtime(&(statbuf->st_ctime)));
    printf("%s ", date);
}

/**
 * @brief: Prints contents of the directory specified by path_name
 * @params: flag_l: denotes the -l flag
 *          flag_a: denotes the -a flag
 */
static void print_contents(char *path_name, int flag_l, int flag_a) {
    DIR *dir = opendir(path_name);
    if (dir == NULL) {
        // Throw warning error
        warning_error_check(0, 0);
        return;
    }

    // Do not free curr_file, may be staticaly allocated
    struct dirent *curr_file;
    while (curr_file = readdir(dir)) {
        // flag_a required for hidden files
        if (!flag_a && curr_file->d_name[0] == '.') {
            continue;
        }

        char *file_path =
            malloc(strlen(path_name) + strlen(curr_file->d_name) + 2);

        sprintf(file_path, "%s/%s", path_name, curr_file->d_name);
        struct stat file_stat;
        lstat(file_path, &file_stat);

        if (flag_l) {
            // TODO: Add extra info for -l
            extra_info_ls(&file_stat);
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

        free(file_path);
    }

    if (!flag_l) {
        printf("\n");
    }
}

void list_ls(CVector *args) {
    optind = 0;  // Mentioned under NOTES section of man 3 optget
    int flag_l = 0, flag_a = 0;
    int opt_ret;

    // Check for flags
    while ((opt_ret = getopt(args->used, args->vector, "al")) != -1) {
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

    for (int i = 1; i < args->used; i++) {
        if (args->vector[i][0] == '-') {
            free(args->vector[i]);
            args->vector[i] = NULL;
        }
    }

    // Flag to check if any directories are passed
    int flag_none = 1;

    for (int i = 1; i < args->used; i++) {
        if (args->vector[i] == NULL) {
            continue;
        }

        flag_none = 0;
        print_contents(replace_tilda(args->vector[i]), flag_l, flag_a);
    }

    // If no args specified, print contents of current file
    if (flag_none) {
        print_contents(".", flag_l, flag_a);
    }
}
