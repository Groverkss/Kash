#ifndef _SIGNAL_HANDLE_
#define _SIGNAL_HANDLE_

/**
 * Linked List mode CVector to store pids + name 
 * Stores pid + name in format: [pid]$[name]
 */
extern CVector *pid_list;

void install_zombie_handler(void);

#endif
