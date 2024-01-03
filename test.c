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
            close(fdIn[1]);
        }
        return pid;
    }
}
int main() {
    int ret;
    char buf[20];

    int fds1[2], fds2[2], stdFd[2];
    stdFd[0] = 0;
    stdFd[1] = 1;

    pid_t pid;

    /// initiating pipes
    init_pipes(fds1, fds2);

    // DO_OR_DIE(pid = fork(), "fork failed");

    // if (pid == 0) {
    //     close(fds1[0]);

    //     dup2(fds1[1], 1);
    //     close(fds1[1]);

    //     execlp("cat", "cat", "/tmp/f", NULL);
    //     return;
    // }
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

    // DO_OR_DIE(pid = fork(), "fork failed");

    // if (pid == 0) {
    //     close(fds2[1]);

    //     dup2(fds2[0], 0);
    //     close(fds2[0]);

    //     execlp("wc", "wc", "-l", NULL);
    // } else {
    //     close(fds2[0]);
    //     close(fds2[1]);

    //     // wait(0);
    //     waitpid(pid, NULL, 0);
    // }
}
