#include "libs.h"
#include "util.h"

int main(int argc, char *argv[]) {

    // TODO: Buffer Size Check + Reallocation
    char *read_buffer = NULL;
    ssize_t nread = 0;
    size_t buffer_len = 0;

    while(nread != -1) {
        display_prompt();
        nread = getline(&read_buffer, &buffer_len, stdin);

        char *save_token_args, *save_args;

        // Parse to indvidual commands
        char *token_args = strtok_r(read_buffer, ";\n", &save_token_args);
        while(token_args) {
            char *args = strtok_r(token_args, " \t", &save_args);

            if(args) {
                if (!strcmp(args, "cd")) {
                    // TODO: CD
                } else if (!strcmp(args, "pwd")) {
                    // TODO: PWD
                } else if (!strcmp(args, "echo")) {
                    // TODO: ECHO
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
}

