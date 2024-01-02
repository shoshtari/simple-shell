#include "shell.h"

/**
 * own_cd - changes the working dir of the current shell executon env
 * @args: target directory
 *
 * Return: -1 one success, 0 otherwise.
 */
int own_cd(split_line_answer command) {
    // TODO
    if (command.count == 1) {
        chdir(getenv("HOME"));
        return -1;
    }
    if (command.count == 2) {
        char* path = malloc(100);
        chdir(command.parts[1]);
        return -1;
    }
    fprintf(stderr, "invalid arguments count");
    return (1);
}
