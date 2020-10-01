#include "libs.h"

static void reset_streams(int STDIN_FD, int STDOUT_FD) {
    /* Return stdin and stdout and close duplicated */
    int stdin_check = dup2(STDIN_FD, STDIN_FILENO);
    int stdout_check = dup2(STDOUT_FD, STDOUT_FILENO);
    fatal_error_check(stdin_check, -1);
    fatal_error_check(stdout_check, -1);
    close(STDIN_FD);
    close(STDOUT_FD);
}

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

    bool use_pipe = true;

    int status_code = 0;
    for (int i = 0; i < args->used; i++) {
        if (!strcmp(args->vector[i], "<")) {
            if (i + 1 == args->used) {
                reset_streams(STDIN_FD, STDOUT_FD);
                return 2;
            }

            int readfile_fd = open(args->vector[i + 1], O_RDONLY);
            if (readfile_fd < 0) {
                perror("Kash");
                reset_streams(STDIN_FD, STDOUT_FD);
                return 3;
            }

            /* Move to the next arguement after read the file name */
            i++;

            /* Close current stdin and replace with readfile_fd */
            int stdinfd = dup2(readfile_fd, STDIN_FILENO);
            if (stdinfd < 0) {
                perror("Kash");
                reset_streams(STDIN_FD, STDOUT_FD);
                return 3;
            }

        } else if (!strcmp(args->vector[i], ">")) {
            if (i + 1 == args->used) {
                reset_streams(STDIN_FD, STDOUT_FD);
                return 2;
            }

            use_pipe = false;
            int writefile_fd = open(args->vector[i + 1],
                    O_WRONLY | O_CREAT | O_TRUNC, 00644);
            if (writefile_fd < 0) {
                perror("Kash");
                reset_streams(STDIN_FD, STDOUT_FD);
                return 3;
            }

            /* Move to the next arguement after read the file name */
            i++;

            /* Close current stdin and replace with readfile_fd */
            int stdoutfd = dup2(writefile_fd, STDOUT_FILENO);

            if (stdoutfd < 0) {
                perror("Kash");
                reset_streams(STDIN_FD, STDOUT_FD);
                return 3;
            }

        } else if (!strcmp(args->vector[i], ">>")) {
            if (i + 1 == args->used) {
                reset_streams(STDIN_FD, STDOUT_FD);
                return 2;
            }

            use_pipe = false;
            int writefile_fd = open(args->vector[i + 1],
                    O_WRONLY | O_CREAT | O_APPEND, 00644);
            if (writefile_fd < 0) {
                perror("Kash");
                reset_streams(STDIN_FD, STDOUT_FD);
                return 3;
            }

            /* Move to the next arguement after read the file name */
            i++;

            /* Close current stdin and replace with readfile_fd */
            int stdoutfd = dup2(writefile_fd, STDOUT_FILENO);

            if (stdoutfd < 0) {
                perror("Kash");
                reset_streams(STDIN_FD, STDOUT_FD);
                return 3;
            }

        } else if (!strcmp(args->vector[i], "|")) {
            status_code |= execute_command(curr, use_pipe);

            /* Reinitialize current args */
            freeCVector(curr);
            curr = malloc(sizeof(CVector));
            initCVector(curr);

            /* Open output stream as stdout */
            int stdout_check = dup2(STDOUT_FD, STDOUT_FILENO);
            fatal_error_check(stdout_check, -1);
            use_pipe = true;
        } else {
            pbCVector(curr, args->vector[i]); 
        }
    }

    /* Don't use pipe for final */
    status_code |= execute_command(curr, false);

    reset_streams(STDIN_FD, STDOUT_FD);
    return status_code;
}

int parse(char *read_buffer) {
    char *save_token_args, *save_args;

    // Parse to indvidual commands
    char *token_args = strtok_r(read_buffer, ";\n", &save_token_args);

    int final_status = 0;
    while(token_args) {
        char *token_dup = strdup(token_args);
        if (token_dup == NULL) {
            // Throw fatal error
            fatal_error_check(0, 0);
        }

        // Convert string to list of args
        CVector *argv = to_args(token_dup);

        int status = parse_args(argv);
        /* status: 0 --> No Problemo */
        /* status: 1 --> Command Error */
        /* status: 2 --> Parse Error */
        /* status: 3 --> File Discriptor Allocation Error */

        if (status) {
            final_status = 1;
        }

        freeCVector(argv);
        free(token_dup);

        // Next list of args
        token_args = strtok_r(NULL, ";\n", &save_token_args);
    }    

    return final_status;
}
