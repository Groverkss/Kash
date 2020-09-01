#ifndef _BUILTIN_
#define _BUILTIN_

// In parse.c, required for ~
extern char *HOME;

// Builtin functions require the save_args for them to work

// Present Working Directory
void present_dir(void);

// Change Directory
void change_dir(CVector *args);

// Echo
void echo(CVector *args);

// List
void list_ls(CVector *args);

#endif
