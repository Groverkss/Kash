#ifndef _PROCLIST_
#define _PROCLIST_

typedef struct process {
    pid_t pid;
    char *name;
    struct process *next;
} process;

extern process *process_list;

void add_pid(pid_t pid, char *name);

void remove_pid(pid_t pid);

char *get_name(pid_t pid);

process *get_by_index(int index);

#endif
