#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int) getpid()); // print the PID about parent process
    int rc = fork(); // start the child process located here. reason why fork -> duplicate the parent process
    // rc < 0 : fork failed, rc = 0 : child process, rc > 0 : parent process

    if (rc < 0) { // fork failed; exit  
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // child process
        printf("hello, I am child (pid:%d)\n", (int) getpid()); // has PID(different about parent PID) 
    } else { // parent process path
        printf("hello, I am parent of %d (pid:%d)\n",rc, (int) getpid());
        // rc -> child PID, getpid -> parent process
    }
    return 0;
}

// result is two case
/*
1 case : 주로 parent process가 먼저 끝나기에 1 case의 결과가 주로 나옴.
hello world (pid : 29146)
hello, I am parent of 29147 (pid : 29146)
hello, I am child (pid : 29147)

2 case
hello world (pid : 29146)
hello, I am child (pid : 29147)
hello, I am parent of 29147 (pid : 29146)

*/