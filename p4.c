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
        close(STDOUT_FILENO);   // closed the terminal
        open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU); // open the file (file : p4.output)

        char *myargs[3];
        myargs[0] = strdup("wc");   // wc program execution
        myargs[1] = strdup("p3.c"); // execution file name
        myargs[2] = NULL;           // must done end is NULL
        execvp(myargs[0], myargs);
        printf("this shouldn't print out");
        // when the success the program then, not print the message. reason why no return. 
    } else { // parent process path
        int wc = wait(NULL); // wait when the child process is done
    }
    return 0;
}

// result
/*
./p4 -> 결과 안 뜸. (-> 실행 결과는 다른 이름을 가진 파일로 생성이 되었기에)
cat p4.output (-> p4.output 안에 있는 값들이 출력된다.)
*/