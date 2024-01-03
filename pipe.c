#include "shell.h"
#include "string.h"

#define DO_OR_DIE(x, s) \
    do {                \
        if ((x) < 0) {  \
            perror(s);  \
            exit(1);    \
        }               \
    } while (0)

int run_with_in_out(split_line_answer command, int* fdIn, int* fdOut) {
    // for (int i = 0; i < command.count; i++) {
    //     printf("%s ", command.parts[i]);
    // }
    // printf("\n");
    // printf("RUN %s %d %d %d %d", command.parts[0], fdIn[0], fdIn[1], fdOut[0], fdOut[1]);
    pid_t pid;
    DO_OR_DIE(pid = fork(), "fork failed");

    if (pid == 0) {
        if (fdIn[1] != 1) {
            close(fdIn[1]);
        }
        if (fdOut[0] != 0) {
            close(fdOut[0]);
        }

        if (fdIn[0] != 0) {
            dup2(fdIn[0], 0);
            close(fdIn[0]);
        }

        if (fdOut[1] != 1) {
            dup2(fdOut[1], 1);
            close(fdOut[1]);
        }

        execvp(command.parts[0], command.parts);
    } else {
        if (fdIn[0] != 0) {
            close(fdIn[0]);
        }
        if (fdIn[1] != 1) {
            close(fdIn[1]);
        }
        return pid;
    }
}

char** copyStringArray(char** a, char** b, size_t size) {
    for (int i = 0; i < size; i++) {
        b[i] = malloc(sizeof(char) * strlen(a[i]));
        strcpy(b[i], a[i]);
    }
}

int execute_with_pipe(split_line_answer command) {
    if (command.count == 0) {
        return -1;
    }
    int startInd = 0;
    split_line_answer segment;
    int lastFD[2], stdFD[2];
    lastFD[0] = 0;
    lastFD[1] = 1;
    stdFD[0] = 0;
    stdFD[1] = 1;

    for (int i = 0; i < command.count; i++) {
        if (strcmp(command.parts[i], "|") == 0) {
            segment.parts = malloc(sizeof(char*) * (i - startInd));
            copyStringArray(command.parts + startInd, segment.parts, i - startInd);
            segment.count = i - startInd;

            // execute_args(segment);
            int fd[2];
            DO_OR_DIE(pipe(fd), "pipe failed");
            run_with_in_out(segment, lastFD, fd);
            lastFD[0] = fd[0];
            lastFD[1] = fd[1];

            startInd = i + 1;
        }
    }

    segment.parts = malloc(sizeof(char*) * (command.count - startInd));
    copyStringArray(command.parts + startInd, segment.parts, command.count - startInd);
    segment.count = command.count - startInd;

    pid_t pid = run_with_in_out(segment, lastFD, stdFD);
    int status;
    waitpid(pid, &status, 0);
    return status - 1;
    // return execute_args(segment);
    // return run_in_new_process(command);
}
