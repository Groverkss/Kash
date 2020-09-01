#include "libs.h"

// Implement dynamic allocation
static const int HOST_SIZE = 128;

char *HOME = NULL;

static int check_prefix(char *path) {
// Check Prefix of string with HOME. If HOME is a prefix,
// replaces the character before start with ~ and
// returns index from where ~ start.
// On fail, returns 0.

// TODO: Special case check for root

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
// TODO: Implement dynamic reallocation
// TODO: Implement memory checks

    char *host_buffer = malloc(HOST_SIZE);
    if (host_buffer == NULL) {
        // Throw fatal error
        fatal_error_check(0, 0);
    }

    gethostname(host_buffer, HOST_SIZE);
    char *user_name = getlogin();
    if (user_name == NULL) {
        // Throw fatal error
        fatal_error_check(0, 0);
    }

    cprintf(BLUE, "<%s@%s:", user_name, host_buffer);

    char *curr_path = get_path();

    if (HOME == NULL) {
        HOME = strdup(curr_path);
        if (HOME == NULL) {
            // Throw fatal error
            fatal_error_check(0, 0);
        }
    }

    cprintf(GREEN, "%s> ", curr_path + check_prefix(curr_path));

    free(host_buffer);
    free(curr_path);
}
