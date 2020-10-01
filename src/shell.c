#include "libs.h"

int main(int argc, char *argv[]) {
    char *read_buffer = NULL;
    ssize_t nread = 0;
    size_t buffer_len = 0;

    install_zombie_handler();

    /* Ignore Ctrl+C and Ctrl+Z */
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    /* Status: 0 No Problemo
     * Status: 1 Some Problemo
     * */
    int current_status = 0;

    while(true) {
        display_prompt(current_status);
        nread = getline(&read_buffer, &buffer_len, stdin);

        if (nread == -1) {
            break;
        }

        add_to_history(read_buffer);

        
        current_status = parse(read_buffer);
    }

    free(read_buffer);
    printf("\n");
}

