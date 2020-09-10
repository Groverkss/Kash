#include "libs.h"

static const int HISTORY_SIZE = 20, DEFAULT_CONTENT = 10;
static const int MAX_BUFFER_SIZE = 2 * HISTORY_SIZE;
static char *HISTORY_NAME = "~/.kash_history";

static char *HISTORY_FILE = NULL;

void init_history(void) {

    /* Initialize history file name */
    HISTORY_FILE = replace_tilda(HISTORY_NAME);

    /* Check if file is present, else create a new file */
    FILE *history_stream = fopen(HISTORY_FILE, "r");
    if (history_stream == NULL) {

        history_stream = fopen(HISTORY_FILE, "w+");
        fprintf(history_stream, "00000\n");
        fclose(history_stream);
    } else {
        fclose(history_stream);

        /* Fix required for opening file again */
        history_stream = fopen(HISTORY_FILE, "a");
        fprintf(history_stream, "\n");
        fclose(history_stream);
    }
}

static CVector *read_filelines(void) {

    /* Read the whole history file */
    FILE *history_file = fopen(HISTORY_FILE, "r");

    /* Initialize a vector to store all lines */
    CVector *history_lines = malloc(sizeof(CVector));
    initCVector(history_lines);

    while(true) {
        char *read_buffer = NULL;
        size_t buffer_size = 0;

        ssize_t nread = getline(&read_buffer, &buffer_size, history_file);
        if (nread == -1) {
            break;
        }

        pbCVector(history_lines, read_buffer);
    }

    fclose(history_file);
    return history_lines;
}

static void reduce_history(void) { 
    CVector *file_lines = read_filelines();

    /* Write required lines to file and delete everything from before */
    FILE *history_file = fopen(HISTORY_FILE, "w");
    fprintf(history_file, "%05d\n", HISTORY_SIZE);

    for (int i = HISTORY_SIZE + 1; i <= MAX_BUFFER_SIZE; i++) {
        fprintf(history_file, "%s", file_lines->vector[i]);
    }

    fclose(history_file);
    freeCVector(file_lines);
}

void print_history(CVector *args) {
    optind = 0;

    /* Get number of lines to print */
    int content = DEFAULT_CONTENT;

    if (args->used >= 2) {
        content = atoi(args->vector[1]);
    }

    /* Content greater than HISTORY_SIZE not allowed */
    if (content > HISTORY_SIZE) {
        content = HISTORY_SIZE;
    }

    CVector *history_lines = read_filelines();

    /* Print last 'content' line */
    int current_line = history_lines->used - 2;
    while(content) {
        if (current_line == 0) {
            break;
        }

        printf("%s", history_lines->vector[current_line]);
        current_line--, content--;
    }

    free(history_lines);
}


void add_to_history(char *command) {

    /* Read number of lines in file */
    FILE *history_stream = fopen(HISTORY_FILE, "r+");
    
    int nread;
    fscanf(history_stream, "%d", &nread);

    /* Increment number of commands stored */
    nread++;
    
    /* Rewind to start of file */
    fseek(history_stream, 0, SEEK_SET);

    /* Write new number */
    fprintf(history_stream, "%05d", nread);

    fclose(history_stream);

    /* Write command at end of file */
    history_stream = fopen(HISTORY_FILE, "a");
    fprintf(history_stream, "%s", command);

    fclose(history_stream);

    if (nread == MAX_BUFFER_SIZE) {
        reduce_history();
    }
}
