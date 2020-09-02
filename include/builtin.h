#ifndef _BUILTIN_
#define _BUILTIN_

/**
 * @brief: Home directory path
 */
extern char *HOME;

/**
 * @brief: Prints present working directory
 * @params: None
 */
void present_dir(void);

/*
 * @brief: Changes directory
 * @params: CVector *args: Vector list of arguements
 */
void change_dir(CVector *args);

/**
 * @brief: Prints given arguements
 * @params: CVector *args: Vector list of arguements
 */
void echo(CVector *args);

/**
 * @brief: Executes a file specified in PATH variable
 *         or a path specified
 * @params: CVector *args: Vector list of arguements
 */
void execute_command(CVector *args);

#endif
