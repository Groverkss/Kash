#include "libs.h"

int main(int argc, char *argv[]) {
    char *read_buffer = NULL;
    ssize_t nread = 0;
    size_t buffer_len = 0;

    install_zombie_handler();
    while(true) {
        display_prompt();
        nread = getline(&read_buffer, &buffer_len, stdin);

        if (nread == -1) {
            break;
        }
        
        parse(read_buffer);
    }

    free(read_buffer);
    printf("\n");
}

