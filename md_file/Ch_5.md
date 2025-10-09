# Process API

---

## Major Functions

> **fork(), wait(), exec()**

---

## **1. fork() : Create a Child Process**

- The **child process** has the **same memory contents** as the parent.  
  → Child process는 parent의 내용을 **복제(duplicate, replicate)** 한다.

- The **child process** is allocated in a **separate memory space**.  
  → 동일한 내용을 가지지만, 서로 **독립적인 메모리 공간**에 존재한다.

- The **child process** has its own registers and program counter (PC).  
  → 레지스터와 PC는 동일한 값을 가지지만, **PID가 다르기 때문에 return 값이 다르고 이후의 동작은 독립적**이다.

- Parent process는 원래의 PID를 유지하고, fork로 생성된 Child process는 **새로운 고유한 PID**를 가진다.

---

### fork() Result Types

| Return Code (rc) | Meaning | Description |
|------------------|----------|--------------|
| **rc = 0** | Child Process | 성공적으로 생성된 자식 프로세스 |
| **rc > 0** | Parent Process | 성공적으로 자식 생성 후 부모 프로세스 실행 |
| **rc = -1** | Failure | 프로세스 생성 실패 |

---

### Reference Code : `p1.c`

```c
int rc = fork();
if (rc < 0) {
    // fork failed
} else if (rc == 0) {
    // child process
} else {
    // parent process
}
```

- `getPID()` → 현재 실행 중인 process의 PID를 가져옴.  
- **fork()만 한 경우**, parent와 child 사이에 **의존성이 없기 때문에 결과는 두 가지 모두 가능**하다.  
  → 보통 parent가 먼저 끝나지만, child가 먼저 종료될 수도 있다.

---

## **2. wait() : Synchronize Parent and Child**

> Create dependency between parent and child processes.

- When a child process is created, the **parent process waits** until the child **has run and exited**.  
  → `wait()`를 사용하면, **child process가 종료될 때까지 parent process는 대기**한다.

- Without `wait()`, the parent and child have **no dependency**.

---

### Example

Child process가 완료된 후, parent process가 완료되어야 하는 상황일 때 `wait()`을 사용한다.

#### Reference Code : `p2.c`
```c
int rc = fork();
if (rc == 0) {
    printf("hello");
} else {
    int wc = wait(NULL);
    printf("goodbye (wc: %d)", wc);
}
```

- Parent 코드 부분에 `int wc = wait(NULL)`을 추가하면,  
  → Child process가 완료된 후 `wc`에 **종료된 child의 PID**가 저장된다.

- ⚠️ **주의사항:** Child가 여러 개일 경우, **하나만 종료되어도 wait()는 return**한다.  
  → 모든 child를 기다리려면 **loop 형태로 wait()**을 여러 번 호출해야 한다.

---

## **3. exec() : Run a New Program**

> The caller wants to run a new program that is **different from itself.**

- OS loads a **new binary image**, initializes a **new stack**, and a **new heap**.  
- Once called, `exec()` **does not return** — it replaces the current process image.

---

### Key Points
- `exec()` replaces the currently running process with a **new program**.  
- If `exec()` returns, it means an **error** occurred.  
- Usually used after `fork()` — so the **child process executes the new program**.  

### Parameters
1. **The name of the binary file**  
2. **The array of arguments**  

---

### Reference Code : `p3.c`

```c
char *myargs[3];
myargs[0] = strdup("wc");       // binary name
myargs[1] = strdup("p3.c");     // argument (filename)
myargs[2] = NULL;               // argv 배열의 끝은 NULL

execvp(myargs[0], myargs);      // wc program 실행
```

**Meaning:**  
`p3.c` 파일을 인자로 받아 **wc 프로그램을 실행**한다.

---

## **Why Separate fork() and exec()?**

> 이유: Between fork() and exec(), we can insert **I/O redirection or pipeline operations.**

---

### Reference Code : `p4.c`

```c
close(STDOUT_FILENO);
open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

char *myargs[3];
myargs[0] = strdup("wc");
myargs[1] = strdup("p3.c");
myargs[2] = NULL;
execvp(myargs[0], myargs);
```

#### 설명
1. `close(STDOUT_FILENO);` → 터미널 출력을 닫음  
2. `open("./p4.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);` → 새로운 파일을 열거나 생성  
3. `execvp()` → 실행 결과를 **`p4.output` 파일에 출력**함

---

✅ **결론 요약**
- `fork()` → 새로운 프로세스 생성  
- `wait()` → 부모-자식 간 동기화  
- `exec()` → 새 프로그램 실행 (현재 프로세스 교체)  
- `fork() + exec()` → 프로세스 복제 후 다른 프로그램 실행 가능  
