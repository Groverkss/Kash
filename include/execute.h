#ifndef _EXECUTE_
#define _EXECUTE_

/**
 * @brief: Home directory path
 */
extern char *HOME;

/**
 * @brief: Executes a file specified in PATH variable
 *         or a path specified
 * @params: CVector *args: Vector list of arguements
 */
int execute_command(CVector *args, bool use_pipe);

#endif
