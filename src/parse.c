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

        CVector *argv = to_args(token_dup);

        if(argv->used != 0) {
            if (!strcmp(argv->vector[0], "cd")) {
                change_dir(argv);

            } else if (!strcmp(argv->vector[0], "pwd")) {
                present_dir();

            } else if (!strcmp(argv->vector[0], "echo")) {
                echo(argv);

            } else if (!strcmp(argv->vector[0], "ls")) {
                list_ls(argv);

            } else {
                execute_command(argv);
            }
        }

        freeCVector(argv);
        free(token_dup);

        token_args = strtok_r(NULL, ";\n", &save_token_args);
    }    
}
