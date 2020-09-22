#ifndef _BUILTIN_
#define _BUILTIN_

/**
 * @brief: Home directory path
 */
extern char *HOME;

/**
 * @brief: Executes a file specified in PATH variable
 *         or a path specified
 * @params: CVector *args: Vector list of arguements
 */
void execute_command(CVector *args);

#endif
