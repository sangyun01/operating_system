#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int) getpid()); // print the PID about parent process
    int rc = fork(); // start the child process located here. reason why fork -> duplicate the parent process
    // rc < 0 : fork failed, rc = 0 : child process, rc > 0 : parent process

    if (rc < 0) { // fork failed; exit  
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) { // child process
        printf("hello, I am child (pid:%d)\n", (int) getpid()); // has PID(different about parent PID) 
        char *myargs[3];
        myargs[0] = strdup("wc");   // wc program execution
        myargs[1] = strdup("p3.c"); // execution file name
        myargs[2] = NULL;           // must done end is NULL
        execvp(myargs[0], myargs);
        printf("this shouldn't print out");
        // when the success the program then, not print the message. reason why no return. 
    } else { // parent process path
        int wc = wait(NULL); // wait when the child process is done
        printf("hello, I am parent of %d (pid:%d)\n",rc, (int) getpid());
        // rc -> child PID, getpid -> parent process
    }
    return 0;
}

// result -> when the child process is done, than parent process is working
/*
hello world (pid : 29146)
hello, I am child (pid : 29147) // child process
hello, I am parent of 29147 (pid : 29146) // parent process
*/