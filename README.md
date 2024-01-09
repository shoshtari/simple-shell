# Q2
I improved the code we had for shell in Q4 of HW2 and added pipe feature to it!

the main file for this change is pipe.c I have a pipe which is the last command output fd (inital fd is 0) and just update it.
for last command I provide its stdin from lastFD but redirect its output to stderr instead of another pipe.

here is a sample run o    printf("KILL %d\n", pid);
f code:
```
[mpc@Legion5 Q2]$ gcc main.c read_line.c split_line.c execute_args.c own_exit.c own_cd.c pipe.c # I hide its warning
[mpc@Legion5 Q2]$ ./a.out
Prompt$ ls
a.out  execute_args.c  LICENSE  main.c  own_cd.c  own_exit.c  pipe.c  read_line.c  README.md  shell.h  split_line.c  test.c
Prompt$ ls | grep a
a.out
execute_args.c
main.c
read_line.c
Prompt$ ls | grep a | wc -l
4
Prompt$ ls | grep a | nl 
     1  a.out
     2  execute_args.c
     3  main.c
     4  read_line.c
Prompt$ 
```

