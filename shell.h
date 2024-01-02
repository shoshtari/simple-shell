/*---LIBRARIES---*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define TOK_DELIM " \t\r\n\a\""

struct split_line_answer {
    char **parts;
    int count;
};

typedef struct split_line_answer split_line_answer;

/* Necessary functions */
char *read_line(void);
split_line_answer split_line(char *line);
int execute_with_pipe(split_line_answer command);

/*---Builtin func---*/
int own_cd(split_line_answer command);
int own_exit(split_line_answer command);