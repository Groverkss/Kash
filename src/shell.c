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
    }
}
