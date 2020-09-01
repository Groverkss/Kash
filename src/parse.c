#include "libs.h"

void parse(char *read_buffer) {
    char *save_token_args, *save_args;

    // Parse to indvidual commands
    char *token_args = strtok_r(read_buffer, ";\n", &save_token_args);
    while(token_args) {
        char *args = strtok_r(token_args, " \t", &save_args);

        if(args) {
            if (!strcmp(args, "cd")) {
                change_dir(&save_args);

            } else if (!strcmp(args, "pwd")) {
                present_dir();

            } else if (!strcmp(args, "echo")) {
                echo(&save_args);

            } else if (!strcmp(args, "ls")) {
                // TODO: LS
                
            } else {
                // TODO: EXEC COMMANDS
                
            }

            // Use this to move foward in the args
            // args = strtok_r(NULL, " \t", &save_args);
        }

        token_args = strtok_r(NULL, ";\n", &save_token_args);
    }    
}
