#pragma once

#define	STDIN_FILENO 0
#define	STDOUT_FILENO 1
#define	STDERR_FILENO 2

#define X_OK 1 /* Test if a file can be executed */
#define W_OK 2 /* Test if a file can be written to */
#define R_OK 4 /* Test if a file can be read */
#define F_OK 0 /* Test if a file exists */

int access(const char *path, int mode);