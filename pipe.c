#include "shell.h"
#include "string.h"

// void runCommands(char** commands) {
//     int i;
//     for (i = 0; *(commands + i + 1); i++) {
//         int pd[2];
//         pipe(pd);

//         if (!fork()) {
//             dup2(pd[1], 1);
//             runCommand(commands[i]);
//             return;
//         }

//         dup2(pd[0], 0);
//         close(pd[1]);
//     }

//     runCommand(commands[i]);
// }

char** copyStringArray(char** a, char** b, size_t size) {
    for (int i = 0; i < size; i++) {
        b[i] = malloc(sizeof(char) * strlen(a[i]));
        strcpy(b[i], a[i]);
    }
}

int execute_with_pipe(split_line_answer command) {
    int startInd = 0;
    split_line_answer segment;

    for (int i = 0; i < command.count; i++) {
        if (strcmp(command.parts[i], "|") == 0) {
            segment.parts = malloc(sizeof(char*) * (i - startInd));
            copyStringArray(command.parts + startInd, segment.parts, i - startInd);
            segment.count = i - startInd;

            execute_args(segment);
            startInd = i + 1;
        }
    }

    segment.parts = malloc(sizeof(char*) * (command.count - startInd));
    copyStringArray(command.parts + startInd, segment.parts, command.count - startInd);
    segment.count = command.count - startInd;

    return execute_args(segment);
    // return run_in_new_process(command);
}
