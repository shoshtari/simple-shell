#include "shell.h"

split_line_answer split_line(char *line) {
    int bufsize = 64;
    int i = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "allocation error in split_line: tokens\n");
        exit(EXIT_FAILURE);
    }
    token = strtok(line, TOK_DELIM);
    int count = 0;
    while (token != NULL) {
        /* handle comments */
        if (token[0] == '#') {
            break;
        }
        count++;
        tokens[i] = token;
        i++;
        if (i >= bufsize) {
            bufsize += bufsize;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "reallocation error in split_line: tokens");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, TOK_DELIM);
    }
    tokens[i] = NULL;
    // printf("%d\n", count);
    split_line_answer answer;
    answer.parts = tokens;
    answer.count = count;
    return answer;
}