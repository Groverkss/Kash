#include "libs.h"

typedef void info_handler(int, siginfo_t*, void*);

info_handler* install_signal(int signum, info_handler* handler) {
    struct sigaction action, old_action;

    memset(&action, 0, sizeof(struct sigaction));
    action.sa_sigaction = handler;
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags =
        SA_RESTART | SA_SIGINFO; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
        perror("Kash");
    return (old_action.sa_sigaction);
}

static void child_exit_handler(int sig, siginfo_t* info, void* vp) {
    int wstatus;
    pid_t child_pid;

    bool found = false;

    while ((child_pid = waitpid(-1, &wstatus, WNOHANG | WUNTRACED)) > 0) {
        found = true;
        if (WIFEXITED(wstatus)) {
            printf("\n%s with pid %d exited normally\n",
                   "Process", child_pid);
        } else {
            printf("\n%s with pid %d exited normally\n",
                   "Process", child_pid);
        }
    }

    if (found) {
        display_prompt();
    }
}

void install_zombie_handler(void) {
    install_signal(SIGCHLD, child_exit_handler);
}
