#include "libs.h"

static const int PATH_SIZE = 256;

void cprintf(const char *color, const char *format, ...) {
    printf("%s", color);

    va_list argptr;
    va_start(argptr, format);
    vprintf(format, argptr);
    va_end(argptr);

    printf("%s", RESET);
}

int fatal_error_check(int out_check, int error_out) {
    if (out_check == error_out) {
        perror("Kash");
        exit(1);
    }

    return 0;
}

int warning_error_check(int out_check, int error_out) {
    if (out_check == error_out) {
        perror("Kash");
        return 1;
    }

    return 0;
}

char *get_path(void) {
    // TODO: Implement dynammic reallocation
    char *curr_path = malloc(PATH_SIZE);
    getcwd(curr_path, PATH_SIZE);
    return curr_path;
}

char *replace_tilda(char *path_name) {
    char *actual_path = NULL;

    if (path_name[0] == '~' &&
            (strlen(path_name) == 1 || path_name[1] == '/')) {
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

CVector *get_stat_args(pid_t pid) {
    char proc_path[64];
    sprintf(proc_path, "/proc/%d/stat", pid);

    FILE *file = fopen(proc_path, "r");
    if (file == NULL) {
        return NULL;
    }


    char *buffer = NULL;
    size_t buffer_size = 0;

    ssize_t nread = getline(&buffer, &buffer_size, file);
    if (nread == -1) {
        return NULL;    
    }

    char exe_path[64];
    sprintf(exe_path, "/proc/%d/exe", pid);
    
    // TODO: Dont use constants
    // TODO: Implement dynamic reallocation
    char *exe = malloc(256);
    nread = readlink(exe_path, exe, 256); 

    // Dont do anything if you cant read. NULL is returned for Zombie process
    if (nread == -1) {
        free(exe);
        exe = NULL;
    }

    CVector *ret = to_args(buffer);
    pbCVector(ret, exe);

    free(buffer);

    return ret;
}
