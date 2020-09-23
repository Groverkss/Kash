#include "libs.h"

process *process_list = NULL;

void add_pid(pid_t pid, char *name) {
    char *name_dup = strdup(name);

    if (process_list == NULL) {
        process_list = malloc(sizeof(process));

        process_list->pid = pid;
        process_list->name = name;
        process_list->next = NULL; 
    } else {
        process *curr = process_list;
        while(curr->next != NULL) {
            curr = curr->next;
        }

        curr->next = malloc(sizeof(process));
        curr = curr->next;
        curr->pid = pid;
        curr->name = name;
        curr->next = NULL;
    }
}

void remove_pid(pid_t pid) {
    process *prev = process_list;
    for (process *curr = process_list; curr != NULL; curr = curr->next) {
        if (curr->pid == pid) {
            if (curr == prev) {
                process_list = process_list->next;
                free(curr);
                break;
            }

            prev->next = curr->next;
            free(curr->name);
            free(curr);
            break;
        }
        prev = curr; 
    }
}

char *get_name(pid_t pid) {
    if (process_list == NULL) {
        return NULL;
    }

    for (process *curr = process_list; curr != NULL; curr = curr->next) {
        if (curr->pid == pid) {
            return curr->name;
        }
    }
    return NULL;
}
