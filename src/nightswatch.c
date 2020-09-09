#include "libs.h"

static const char *INTERRUPT_FILE = "/proc/interrupts";
static const char *NEWBORN_FILE = "/proc/loadavg";

static void interrupt_check(void) {
    FILE *interrupt_stream = fopen(INTERRUPT_FILE, "r");

    char *line1 = NULL, *line2 = NULL, *line3 = NULL;
    size_t size1 = 0, size2 = 0, size3 = 0;

    getline(&line1, &size1, interrupt_stream);
    getline(&line2, &size2, interrupt_stream);
    getline(&line3, &size3, interrupt_stream);

    CVector *args1 = to_args(line1);
    CVector *args2 = to_args(line3);

    for (int i = 0; i < args1->used - 1; i++) {
        printw("      %s", args1->vector[i]);
    }
    printw("\n");
    for (int i = 1; i < args1->used; i++) {
        int check = atoi(args2->vector[i]);
        printw("%10d", check);
    }
    printw("\n");

    fclose(interrupt_stream);
}

static void newborn_check(void) { 
    FILE *newborn_stream = fopen(NEWBORN_FILE, "r");

    char *line = NULL;
    size_t nread = 0;

    getline(&line, &nread, newborn_stream);

    CVector *args = to_args(line);

    printw("%s\n", args->vector[args->used - 1]);

    fclose(newborn_stream);
}

void nightswatch(CVector *args) { 
    int n_sec = -1;

    /*Mention under NOTES section of man 3 optget*/
    optind = 0;

    int opt_ret;

    /* Check for flag */
    while((opt_ret = getopt(args->used, args->vector, "n:")) != -1) {
        if (opt_ret == 'n') {
            if (optarg != NULL) {
               n_sec = atoi(optarg); 
            }
        } else {
            fprintf(stderr, "Usage: nightswatch [option] <command>\n");
            return;
        }
    }

    if (n_sec < 1) {
        fprintf(stderr, "Not a valid arguement for n\n");
    }

    int command_type = -1;

    for (int i = 1; i < args->used; i++) {
        if (!strcmp("newborn", args->vector[i])) {
            command_type = 1;
        } else if (!strcmp("interrupt", args->vector[i])) {
            command_type = 0;
        }
    }

    if (command_type == -1) {
        fprintf(stderr, "Usage: nightswatch [options] <command>");
        return;
    }

    /* Open ncurses window */
    initscr();
    clear();
    raw();

    int timer = n_sec;

    while(true) {
        timeout(0);
        int key = getch();
        if (key == 'q') {
            break;
        }

        if (timer == n_sec) {
            if (command_type == 0) {
                interrupt_check();
            } else {
                newborn_check();
            }
            timer = 0;
        }
        sleep(1);
        timer += 1;
    }

    /* Close ncurses window */
    endwin();
}
