#include "libs.h"

typedef void info_handler(int, siginfo_t*, void*);

CVector *pid_list;

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

    while ((child_pid = waitpid(-1, &wstatus,
                    WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
        found = true;
        char *name_pid = NULL;

        if (WIFEXITED(wstatus)) {
            name_pid = remove_from_CVector(pid_list, child_pid, true);
            fprintf(stderr, "\n%s with pid %d exited normally\n",
                    name_pid, child_pid);
        } else if (WIFSTOPPED(wstatus)) {
            name_pid = remove_from_CVector(pid_list, child_pid, false);
            fprintf(stderr, "\n%s with pid %d suspended normally\n",
                    name_pid, child_pid);
        } else if (WIFCONTINUED(wstatus)) {
            name_pid = remove_from_CVector(pid_list, child_pid, false);
            fprintf(stderr, "\n%s with pid %d continued normally\n",
                    name_pid, child_pid);
        } else {
            name_pid = remove_from_CVector(pid_list, child_pid, true);
            fprintf(stderr, "\n%s with pid %d did not exit normally\n",
                    name_pid, child_pid);
        }

        free(name_pid);
    }

    if (found) {
        display_prompt();
    }
}

void install_zombie_handler(void) {
    /* Initialize pidlist */
    pid_list = malloc(sizeof(CVector *));
    initCVector(pid_list);
    install_signal(SIGCHLD, child_exit_handler);
}
