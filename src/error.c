#include "libs.h"

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
