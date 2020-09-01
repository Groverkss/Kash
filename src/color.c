#include "libs.h"

void cprintf(const char* color, const char *format, ...) {
    printf("%s", color);

    va_list argptr; 
    va_start(argptr, format);
    vprintf(format, argptr);
    va_end(argptr);

    printf("%s", RESET);
}
