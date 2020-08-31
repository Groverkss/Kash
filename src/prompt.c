#include "util.h"
#include "libs.h"

// Implement dynamic allocation
static const int HOST_SIZE = 128;
static const int PATH_SIZE = 128;

static char *HOME = NULL;

static int check_prefix(char *path) {
// Check Prefix of string with HOME. If HOME is a prefix,
// replaces the character before start with ~ and
// returns index from where ~ start.
// On fail, returns 0.

    int home_len = strlen(HOME);
    int path_len = strlen(path);
    if (home_len > path_len) {
        return 0;
    }

    for (int i = 0; i < home_len; i++) {
        if (HOME[i] != path[i]) {
            return 0;
        }
    }

    path[home_len - 1] = '~';
    return home_len - 1;
}

void display_prompt(void) {
// Implement dynamic reallocation
    char *host_buffer = malloc(HOST_SIZE);

    gethostname(host_buffer, HOST_SIZE);
    char *user_name = getlogin();

    cprintf(BLUE, "<%s@%s:", user_name, host_buffer);

    char *curr_path = malloc(PATH_SIZE);
    getcwd(curr_path, PATH_SIZE);

    if (HOME == NULL) {
        HOME = strdup(curr_path);
    }

    cprintf(GREEN, "%s> ", curr_path + check_prefix(curr_path));

    free(host_buffer);
    free(curr_path);
}
