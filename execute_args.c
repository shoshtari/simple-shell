#include "shell.h"

int run_in_new_process(split_line_answer command) {
    // since exec will replace program memory space with the new one we called, it should run in a new process to avoid losing our bash

    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Error on Forking");
        return -10;
    }
    if (pid == 0) {
        execvp(command.parts[0], command.parts);
    }

    if (pid > 0) {
        int status;
        waitpid(pid, &status, NULL);
        return status - 1;  // since in linux zero means ok but we want -1
    }
}
int execute_args(split_line_answer command) {
    // for (int i = 0; i < command.count; i++) {
    //     printf("RUNNING %s", command.parts[i]);
    // }
    // printf("\n");

    if (command.count == 0) {
        return -1;
    }
    char **args = command.parts;

    int status;

    char *builtin_func_list[] = {
        // you should add something here
        "exit",
        "cd"};
    int (*builtin_func[])(split_line_answer) = {
        // you should add something here
        &own_exit,
        &own_cd,
    };

    long unsigned int i = 0;
    for (int i = 0; i < sizeof(builtin_func_list) / sizeof(char *); i++) {
        /* if there is a match execute the builtin command */
        if (strcmp(args[0], builtin_func_list[i]) == 0) {
            return ((*builtin_func[i])(command));
        }
    }
    // return run_with_pipe(command);
    return run_in_new_process(command);
}
