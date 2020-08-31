#ifndef _UTIL_
#define _UTIL_

// Prompt functions
void display_prompt(void);

// Color macros
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define PINK    "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"
#define RESET   "\x1B[0m"

void cprintf(const char *color, const char *format, ...);

#endif
