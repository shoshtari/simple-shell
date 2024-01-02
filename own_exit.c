#include "shell.h"

/**
 * own_exit - couses normal process termination
 * @args: empty args
 *
 * Return: 0 to terminate the process
 */
int own_exit(split_line_answer command) {
    /* exit with status */
    char** args = command.parts;
    if (args[1]) {
        return (atoi(args[1]));
    }
    /* exit success */
    else {
        return (0);
    }
}
