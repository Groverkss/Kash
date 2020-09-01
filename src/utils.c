#include "libs.h"

static const int PATH_SIZE = 128;

void cprintf(const char* color, const char *format, ...) {
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
