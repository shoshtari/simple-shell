#include "shell.h"

int main(int argc, char *argv[]) {
    char *line;
    split_line_answer command;
    int status = -1;
    do {
        printf("Prompt$ ");         /* print prompt symbol */
        line = read_line();         /* read line from stdin */
        command = split_line(line); /* tokenize line */
        status = execute_with_pipe(command);
        /* avoid memory leaks */
        free(line);
        free(command.parts);
        /* exit with status */
        if (status >= 0) {
            exit(status);
        }
    } while (status == -1);
}