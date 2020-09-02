#include "libs.h"

typedef void info_handler(int, siginfo_t *, void *);

info_handler* install_signal(int signum, info_handler* handler) {
    struct sigaction action, old_action;

    memset(&action, 0, sizeof(struct sigaction));
    action.sa_sigaction = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART|SA_SIGINFO; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
        perror("Kash");
    return (old_action.sa_sigaction);
}

static void child_exit_handler(int sig, siginfo_t* info, void* vp) {
    int wstatus;
    pid_t child_pid;

    if ((child_pid = waitpid(-1, &wstatus, WNOHANG | WUNTRACED)) > 0) { 
        if (WIFEXITED(wstatus)) {
            printf("\nProcess with pid %d exited normally\n", child_pid);
            display_prompt();
        } else {
            printf("\nProcess with pid %d did not exit normally\n", child_pid);
            display_prompt();
        }
    }
}

void install_zombie_handler(void) {
    install_signal(SIGCHLD, child_exit_handler);
} 

