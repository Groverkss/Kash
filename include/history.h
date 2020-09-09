#ifndef _HISTORY_
#define _HISTORY_

/**
 * @brief: Initializes the history file for reading
 */
void init_history(void);

/**
 * @brief: Return history of last n lines
 * @params: CVector *args: Vector list of arguements 
 */
void print_history(CVector *args);

/**
 * @brief: Adds a string to history
 * @params: char *command: String to add to history
 */
void add_to_history(char *command);

#endif
