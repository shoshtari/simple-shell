#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define DO_OR_DIE(x, s) \
    do {                \
        if ((x) < 0) {  \
            perror(s);  \
            exit(1);    \
        }               \
    } while (0)

void init_pipes(int* fds1, int* fds2) {
    DO_OR_DIE(pipe(fds1), "could not pipe");
    DO_OR_DIE(pipe(fds2), "could not pipe");
}

int run(char** cmd, int* fdIn, int* fdOut) {
    pid_t pid;
    DO_OR_DIE(pid = fork(), "fork failed");

    if (pid == 0) {
        close(fdIn[1]);
        close(fdOut[0]);

        if (fdIn[0] != 0) {
            dup2(fdIn[0], 0);
            close(fdIn[0]);
        }

        if (fdOut[1] != 1) {
            dup2(fdOut[1], 1);
            close(fdOut[1]);
        }

        execvp(cmd[0], cmd);
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

void run_all(char*** commands, int n) {
    int lastFD[2], stdFD[2];
    lastFD[0] = 0;
    lastFD[1] = 1;
    stdFD[0] = 0;
    stdFD[1] = 1;

    for (int i = 0; i < n - 1; i++) {
        int fd[2];
        DO_OR_DIE(pipe(fd), "could not pipe");
        run(commands[i], lastFD, fd);
        lastFD[0] = fd[0];
        lastFD[1] = fd[1];
    }

    waitpid(run(commands[n - 1], lastFD, stdFD), NULL, 0);
}

int run_hard() {
    char buf[20];

    int fds1[2], fds2[2], stdFd[2];
    stdFd[0] = 0;
    stdFd[1] = 1;

    pid_t pid;

    init_pipes(fds1, fds2);

    char** firstCommand = (char**)malloc(sizeof(char*) * 2);
    firstCommand[0] = "cat";
    firstCommand[1] = "/tmp/f";
    run(firstCommand, stdFd, fds1);

    char** secondCommand = (char**)malloc(sizeof(char*) * 1);
    secondCommand[0] = "nl";
    run(secondCommand, fds1, fds2);

    char** thirdCommand = (char**)malloc(sizeof(char*) * 2);
    thirdCommand[0] = "wc";
    thirdCommand[1] = "-l";
    pid = run(thirdCommand, fds2, stdFd);
    waitpid(pid, NULL, 0);
}

int main() {
    char*** commands = (char***)malloc(sizeof(char**) * 3);
    commands[0] = (char**)malloc(sizeof(char*) * 2);
    commands[0][0] = "cat";
    commands[0][1] = "/tmp/f";
    commands[1] = (char**)malloc(sizeof(char*) * 1);
    commands[1][0] = "nl";
    commands[2] = (char**)malloc(sizeof(char*) * 2);
    commands[2][0] = "wc";
    commands[2][1] = "-l";
    run_all(commands, 3);

    return 0;
}