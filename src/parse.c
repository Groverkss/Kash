#include "libs.h"

static int parse_args(CVector *args) {
    CVector *curr = malloc(sizeof(CVector));
    if (curr == NULL) {
        // Throw fatal error
        fatal_error_check(0, 0);
    }

    initCVector(curr);
    int STDIN_FD = dup(STDIN_FILENO);
    int STDOUT_FD = dup(STDOUT_FILENO);

    if (STDIN_FD < 0 || STDOUT_FD < 0) {
        return 3;
    }

    for (int i = 0; i < args->used; i++) {
        if (!strcmp(args->vector[i], "<")) {
            if (i + 1 == args->used) {
                return 2;
            }

            int readfile_fd = open(args->vector[i + 1], O_RDONLY);
            if (readfile_fd < 0) {
                return 3;
            }

            /* Move to the next arguement after read the file name */
            i++;

            /* Close current stdin and replace with readfile_fd */
            int stdinfd = dup2(readfile_fd, STDIN_FILENO);
            if (stdinfd < 0) {
                /* TODO: Reset stdin */
                return 3;
            }

        } else if (!strcmp(args->vector[i], ">")) {
            if (i + 1 == args->used) {
                return 2;
            }

            int writefile_fd = open(args->vector[i + 1],
                    O_WRONLY | O_CREAT, 00644);
            if (writefile_fd < 0) {
                return 3;
            }

            /* Move to the next arguement after read the file name */
            i++;

            /* Close current stdin and replace with readfile_fd */
            int stdoutfd = dup2(writefile_fd, STDOUT_FILENO);

            if (stdoutfd < 0) {
                /* TODO: Reset stdout */
                return 3;
            }

        } else if (!strcmp(args->vector[i], "|")) {
            execute_command(curr);

            /* Reinitialize current args */
            freeCVector(curr);
            curr = malloc(sizeof(CVector));
            initCVector(curr);

            /* Open output stream as stdout */
            int stdout_check = dup2(STDOUT_FD, STDOUT_FILENO);
            fatal_error_check(stdout_check, -1);
        } else {
            pbCVector(curr, args->vector[i]); 
        }
    }

    execute_command(curr);

    /* Print the output from the final pipe */
    int BUFFER_SZ = 512;
    char *read_buffer[512];
    ssize_t nread;

    while(true) {
        nread = read(STDIN_FILENO, &read_buffer, BUFFER_SZ);
        if (nread < 1) {
            break;
        } 

        write(STDOUT_FILENO, &read_buffer, nread);
    }

    /* Return stdin and stdout and close duplicated */
    int stdin_check = dup2(STDIN_FD, STDIN_FILENO);
    int stdout_check = dup2(STDOUT_FD, STDOUT_FILENO);
    fatal_error_check(stdin_check, -1);
    fatal_error_check(stdout_check, -1);
    close(STDIN_FD);
    close(STDOUT_FD);

    return 0;
}

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

        int status = parse_args(argv);
        /* status: 2 --> Parse Error */
        /* status: 3 --> File Discriptor Allocation Error */

        freeCVector(argv);
        free(token_dup);

        // Next list of args
        token_args = strtok_r(NULL, ";\n", &save_token_args);
    }    
}
