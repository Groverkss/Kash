#include "libs.h"

void parse(char *read_buffer) {
    char *save_token_args, *save_args;

    // Parse to indvidual commands
    char *token_args = strtok_r(read_buffer, ";\n", &save_token_args);

    while(token_args) {
        char *token_dup = strdup(token_args);
        if (token_dup == NULL) {
            // Throw fatal error
            fatal_error_check(0, 0);
        }

        // Convert string to list of args
        CVector *argv = to_args(token_dup);

        // Execute list of args
        if(argv->used != 0) {
            if (!strcmp(argv->vector[0], "cd")) {
                change_dir(argv);

            } else if (!strcmp(argv->vector[0], "pwd")) {
                present_dir();
            } else if (!strcmp(argv->vector[0], "echo")) {
                builtin_echo(argv);
            } else if (!strcmp(argv->vector[0], "ls")) {
                list_ls(argv);
            } else if (!strcmp(argv->vector[0], "pinfo")){
                process_info(argv);
            } else if (!strcmp(argv->vector[0], "nightswatch")) {
                nightswatch(argv);
            } else if (!strcmp(argv->vector[0], "history")) {
                print_history(argv);
            }
            else {
                execute_command(argv);
            }
        }

        freeCVector(argv);
        free(token_dup);

        // Next list of args
        token_args = strtok_r(NULL, ";\n", &save_token_args);
    }    
}
