#ifndef _UTIL_
#define _UTIL_

// Prompt functions

void display_prompt(void);

// Color macros

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define PINK "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE "\x1B[37m"
#define RESET "\x1B[0m"

void cprintf(const char *color, const char *format, ...);

// Error handling

// out_check == error_check --> exits with return code 1
// else returns 0
int fatal_error_check(int out_check, int error_out);

// out_check == error_check --> returns 1
// else returns 0
int warning_error_check(int out_check, int error_out);

// Parsing

void parse(char *read_buffer);

// Dynamic Allocation
// Returns Pathname with dynammic allocation of path
char *get_path(void);

/**
 * @brief: Creates a new string with tilda replcaed with HOME
 * @params: char *path_name: a string to modify
 * @return: A pointer to the modified string
 */
char *replace_tilda(char *path_name);

#endif
