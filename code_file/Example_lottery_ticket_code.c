#include <sys/random.h>
#include <stdlib.h>
#include <stdio.h>

// Node structure definition
typedef struct node {
    int pid;
    int tickets;
    struct node *next;
} node_t;

// Total number of tickets
int totaltickets = 400;
// Head pointer of queue
node_t *head;

int main() {
    // 1️⃣ 모든 노드를 먼저 생성
    node_t *C = (node_t *)malloc(sizeof(node_t));
    node_t *B = (node_t *)malloc(sizeof(node_t));
    node_t *A = (node_t *)malloc(sizeof(node_t));
    // Queue -> linked list structure

    // 2️⃣ 각 노드 초기화
    C->pid = 12;
    C->tickets = 250;
    C->next = NULL;   // 마지막 노드는 NULL

    B->pid = 11;
    B->tickets = 50;
    B->next = C;      // B -> C

    A->pid = 10;
    A->tickets = 100;
    A->next = B;      // A -> B

    head = A;         // head는 A를 가리킴
    
    int counter = 0;
    // counter: used to track if we’ve found the winner yet
    // 어떤 process가 lottery 실행인지 확인하기 위한 count용, 최초 0 초기화

    int winner = getrandom(0, totaltickets);
    // winner: use some call to a random number generator to
    // get a value, between 0 and the total # of tickets
    // 0 ~ 399 중 하나의 숫자가 나오며 이때 130이라 가정을 하자.

    node_t *current = head;
    // current: use this to walk through the list of jobs
    // 최초에 head는 A를 가르키고 있다.

    // loop until the sum of ticket values is > the winner
    while (current) {
        // 1. A의 address : 1000
        // 2. B의 address : 1100
        counter = counter + current->tickets;
        // 1. 0 + 100 = 100
        // 2. 100 + 50 = 150
        if (counter > winner)
            // 1. 100 > 130 -> False
            // 2. 150 > 130 -> True
            break;  // found the winner
                    // Process B를 실행한다. 
        current = current->next;
        // 1. A -> B로 이동
    }

    free(A);
    free(B);
    free(C);
}